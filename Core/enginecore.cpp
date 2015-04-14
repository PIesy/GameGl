#include "enginecore.h"
#include "Helpers/helpers.h"
#include <SDL2/SDL.h>
#include "IO/ioevents.h"
#include "Logger/logger.h"

void SDLpoller(EventHandler* handler);

EngineCore::EngineCore(EngineInitializer initializer)
{
    data = new CoreData;
    initModules();
    initApis(initializer);
}

EngineCore::~EngineCore()
{
    delete data;
}

void EngineCore::initApis(EngineInitializer initializer)
{
    ModuleInterface* module;
    for (ModuleApiPair& pair : initializer.apis)
    {
        module = GetModule(pair.module);
        module->setApi(pair.api);
    }
}

void EngineCore::initModules()
{
    GraphicsModule* g = new GraphicsModule;
    AttachModule(Modules::Video, g);
}

ModuleInterface* EngineCore::GetModule(Modules name)
{
    return modules.at(static_cast<int>(name));
}

void EngineCore::AttachModule(Modules name, ModuleInterface* module)
{
    module->setEngine(this);
    modules.insert({integral(name), module});
}

EventHandler& EngineCore::getEventHandler()
{
    return eventHandler;
}

void EngineCore::Start()
{
    startCoreThread();
}

void EngineCore::Terminate()
{
    data->terminate = true;
    GlobalBroadcaster::NotifyAll(0);
    worker.Terminate();
}

void EngineCore::WaitEnd()
{
    worker.Join();
}

void EngineCore::startCoreThread()
{
    Action<EventHandler*> action(std::bind(SDLpoller, std::placeholders::_1));
    worker.setTask(action, (void*)&eventHandler);
}

void SDLpoller(EventHandler* handler)
{
    Logger::Log("event poller started");
    SDL_Event event;
    WindowData w;
    MouseData m;
    KeyboardData k;
    int newEvent = 0;

    while(1)
    {
        newEvent = SDL_PollEvent(&event);
        if (newEvent)
        {
            switch(event.type) {
            case SDL_WINDOWEVENT:
                w.eventType = event.window.event == SDL_WINDOWEVENT_CLOSE ? w.Type::Close : w.Type::Resize;
                handler->ThrowEvent(new WindowEvent(w));
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                k.isPressed = event.key.state == SDL_PRESSED;
                k.scancode = event.key.keysym.scancode;
                k.windowId = event.key.windowID;
                handler->ThrowEvent(new KeyboardEvent(k));
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
                m.eventType = m.Type::Motion;
                handler->ThrowEvent(new MouseEvent(m));
                break;
            }
        }
        SDL_Delay(1);
    }
    Logger::Log("event poller stopped");
}
