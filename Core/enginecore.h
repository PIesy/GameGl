#ifndef ENGINECORE_H
#define ENGINECORE_H

#include <forward_list>
#include "engineinterface.h"
#include "enginecoreincludes.h"
#include "service/service.h"
#include "../Helpers/shared_thread_local_wrapper.h"
#include "executor.h"
#include "threadpool.h"

struct CoreData
{
    std::list<Worker> workerList;
    std::mutex mutex;
    std::unordered_map<int, std::unique_ptr<ModuleInterface>> modules;
    std::forward_list<ServiceContainer> services;
};

class EngineCore: public EngineInterface
{
    std::unique_ptr<CoreData> data;
    std::unique_ptr<ThreadPool> threadPool;
    std::unique_ptr<EventHandler> eventHandler;
    Worker coreWorker{getClassName<EngineCore>()};
    void initModules();
    void initApis(EngineInitializer initializer);
    bool isInit = false;
public:
    explicit EngineCore(const EngineInitializer& initializer);
    ~EngineCore();
    void AttachModule(ModuleType name, ModuleInterface* module);
    ModuleInterface& GetModule(ModuleType name);
    EventHandler& GetEventHandler();
    Executor& GetExecutor(bool exclusive, const std::string& name) override;
    void Start();
    void Terminate();
    void WaitEnd();
};

namespace core
{
    extern thread_local SharedThreadLocalWrapper<EngineInterface> core;
}

#endif // ENGINECORE_H
