#include "enginecore.h"
#include "../Logger/logger.h"

namespace core
{
    thread_local SharedThreadLocalWrapper<EngineInterface> core;
}

EngineCore::EngineCore(const EngineInitializer& initializer)
{
    coreWorker.Execute(Task{[this, initializer]()
    {
        core::core.Init(*this);
        data = std::make_unique<CoreData>();
        threadPool = std::make_unique<ThreadPool>(ThreadPoolConfig{3, 10});
        eventHandler = std::make_unique<EventHandler>();
        data->workerList.emplace_back("dummy");
        isInit = true;
        initModules();
        initApis(initializer);
    }});
}

EngineCore::~EngineCore()
{
    Terminate();
    WaitEnd();
}

void EngineCore::initApis(EngineInitializer initializer)
{
    ServiceContainer service;

    for (auto& provider : initializer.providers)
    {
        auto module = provider.get()->GetModule();
        data->services.push_front(module->GetServices());
        data->modules.emplace(integral(module->GetType()), std::move(module));
    }
}

void EngineCore::initModules()
{
}

ModuleInterface& EngineCore::GetModule(ModuleType name)
{
    Task getModule;

    auto future = getModule.SetTask([this, name]()
    {
        return std::ref(data->modules.at(integral(name)));
    });
    coreWorker.Execute(getModule);

    std::reference_wrapper<std::unique_ptr<ModuleInterface>> result = future.get();
    return *result.get().get();
}

void EngineCore::AttachModule(ModuleType name, ModuleInterface* module)
{
    coreWorker.Execute(Task{[=]()
    {
        data->modules.emplace(integral(name), std::unique_ptr<ModuleInterface>(module));
    }});
}

EventHandler& EngineCore::GetEventHandler()
{
    return *eventHandler;
}

void EngineCore::Start()
{
    coreWorker.Execute(Task{[this]()
    {
        Logger::Log("Core started");
        for(ServiceContainer& service: data->services)
            service.Start();
    }});
}

void EngineCore::Terminate()
{
    coreWorker.Execute(Task{[this]()
    {
        for (ServiceContainer& service: data->services)
            service.Stop();
    }});
}

void EngineCore::WaitEnd()
{
    Task end{[this]()
    {
        for (ServiceContainer& service: data->services)
            service.Wait();
        Logger::Log("Core stopped");
    }};

    coreWorker.Execute(end);
    end.WaitTillFinished();
}

Executor& EngineCore::GetExecutor(bool exclusive, const std::string& name)
{
    std::lock_guard<std::mutex> guard{data->mutex};

    if (exclusive)
    {
        data->workerList.emplace_back(name);
        return data->workerList.back();
    }
    return *threadPool;
}
