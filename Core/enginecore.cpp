#include "enginecore.h"
#include "Helpers/helpers.h"

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

void EngineCore::BindAction(ActionOld action, int type, int category)
{
    handler.BindAction(action, type, category);
}

void EngineCore::Start()
{
    startCoreThread();
}

void EngineCore::Terminate()
{
    data->terminate = true;
    GlobalBroadcaster::NotifyAll(0);
    handler.Terminate();
}

void EngineCore::WaitEnd()
{
    handler.WaitEnd();
}

void EngineCore::startCoreThread()
{
    handler.Start();
}
