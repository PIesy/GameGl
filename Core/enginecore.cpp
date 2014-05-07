#include "enginecore.h"
#include <iostream>

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
    IModule* module;
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

IModule* EngineCore::GetModule(Modules name)
{
    return modules.at(static_cast<int>(name));
}

void EngineCore::AttachModule(Modules name, IModule* module)
{
    std::pair<int, IModule*> pair(static_cast<int>(name), module);
    modules.insert(pair);
}

void EngineCore::BindAction(Action action, int type, int category)
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
    handler.Terminate();
    worker.Terminate();
}

void EngineCore::WaitEnd()
{
    worker.Join();
}

void EngineCore::startCoreThread()
{
    worker.setWork((void (*)(void*))EventPoller, &handler);
}
