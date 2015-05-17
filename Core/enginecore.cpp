#include "enginecore.h"
#include "Helpers/helpers.h"
#include <SDL2/SDL.h>
#include "Input/inputevents.h"
#include "Logger/logger.h"

void SDLpoller(EventHandler* handler);

EngineCore::EngineCore(EngineInitializer initializer)
{
    initModules();
    initApis(initializer);
}

EngineCore::~EngineCore()
{
    Terminate();
    WaitEnd();
}

void EngineCore::initApis(EngineInitializer initializer)
{
    ServiceContainer service;

    for (ModuleApiPair& pair : initializer.apis)
    {
        GetModule(pair.module).setApi(pair.api);
        service = pair.api->getService();
        if(service)
        {
            data.services.push_front(service);
            if(data.started)
                service.Start();
        }
    }
}

void EngineCore::initModules()
{
    GraphicsModule* g = new GraphicsModule;
    InputModule* i = new InputModule;
    ResourcesModule* r = new ResourcesModule;
    AttachModule(Modules::Video, g);
    AttachModule(Modules::Input, i);
    AttachModule(Modules::Storage, r);
}

ModuleInterface& EngineCore::GetModule(Modules name)
{
    return *modules.at(integral(name));
}

void EngineCore::AttachModule(Modules name, ModuleInterface* module)
{
    module->setEngine(*this);
    modules.insert({integral(name), module});
}

EventHandler& EngineCore::getEventHandler()
{
    return eventHandler;
}

void EngineCore::Start()
{
    data.started = true;
    for(ServiceContainer& service: data.services)
        service.Start();
}

void EngineCore::Terminate()
{
    GlobalBroadcaster::NotifyAll(0);
    for(ServiceContainer& service: data.services)
        service.Stop();
}

void EngineCore::WaitEnd()
{
    for(ServiceContainer& service: data.services)
        service.Wait();
}
