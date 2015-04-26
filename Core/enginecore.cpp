#include "enginecore.h"
#include "Helpers/helpers.h"
#include <SDL2/SDL.h>
#include "Input/inputevents.h"
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
    Service* service;
    ModuleInterface* module;

    for (ModuleApiPair& pair : initializer.apis)
    {
        module = GetModule(pair.module);
        module->setApi(pair.api);
        service = pair.api->getService();
        if(service)
        {
            data->services.push_front(service);
            if(data->started)
                service->Start();
        }
    }
}

void EngineCore::initModules()
{
    GraphicsModule* g = new GraphicsModule;
    InputModule* i = new InputModule;
    AttachModule(Modules::Video, g);
    AttachModule(Modules::Input, i);
}

ModuleInterface* EngineCore::GetModule(Modules name)
{
    return modules.at(integral(name));
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
    data->started = true;
    for(Service*& service: data->services)
        service->Start();
}

void EngineCore::Terminate(bool wait)
{
    GlobalBroadcaster::NotifyAll(0);
    for(Service*& service: data->services)
        service->Stop();
    if(wait)
        for(Service*& service: data->services)
            service->Wait();

}

void EngineCore::WaitEnd()
{
    for(Service*& service: data->services)
        service->Wait();
}

void EngineCore::startCoreThread()
{
    Task task(SDLpoller, &eventHandler);
    worker.setTask(task);
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
