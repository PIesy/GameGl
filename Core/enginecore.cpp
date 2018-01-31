#include "enginecore.h"
#include "../Logger/logger.h"

namespace core
{
    thread_local SharedThreadLocal<EngineInterface> core;
}

auto logger = Logger::GetLogger(getClassName<EngineCore>());

EngineCore::EngineCore(const EngineInitializer& initializer)
{
    coreWorker.Execute(Task{[this, initializer]()
    {
        core::core.Init(*this);
        data = std::make_unique<CoreData>();
        threadPool = std::make_unique<ThreadPool>(ThreadPoolConfig{3, 10});
        eventHandler = std::make_unique<EventHandler>();
        initModules(initializer);
    }});
}

EngineCore::~EngineCore()
{
    Terminate();
    WaitEnd();
}

void EngineCore::initModules(EngineInitializer initializer)
{
    ServiceContainer service;

    for (auto& provider : initializer.providers)
    {
        auto module = provider.get()->GetModule();
        data->services.push_front(module->GetServices());
        data->modules.emplace(integral(module->GetType()), std::move(module));
    }
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

void EngineCore::AttachModule(std::shared_ptr<ModuleProvider> provider)
{
    coreWorker.Execute(Task{[=]()
    {
        auto module = provider.get()->GetModule();
        data->services.push_front(module->GetServices());
        if (isRunning)
            data->services.front().Start();
        data->modules.emplace(integral(module->GetType()), std::move(module));
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
        isRunning = true;
        logger.LogDebug("Core started");
        for (ServiceContainer& service: data->services)
            service.Start();
    }});
}

void EngineCore::Terminate()
{
    coreWorker.Execute(Task{[this]()
    {
        for (ServiceContainer& service: data->services)
            service.Stop();
        for (Worker& worker : data->workerList)
            worker.Terminate();
    }});
}

void EngineCore::WaitEnd()
{
    for (ServiceContainer& service: data->services)
        service.Wait();
    for (Worker& worker : data->workerList)
        worker.Join();
    logger.LogDebug("Core stopped");
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
