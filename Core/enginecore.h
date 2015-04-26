#ifndef ENGINECORE_H
#define ENGINECORE_H

#include "engineinterface.h"
#include "enginecoreincludes.h"

struct CoreData
{
    EngineInterface* core;
    bool started = false;
    std::forward_list<Service*> services;
};

class EngineCore: public EngineInterface
{
    std::unordered_map<int, ModuleInterface*> modules;
    Worker worker;
    CoreData* data;
    EventHandler eventHandler;
    void initModules();
    void initApis(EngineInitializer initializer);
    void startCoreThread();
public:
    EngineCore(EngineInitializer initializer);
    ~EngineCore();
    void AttachModule(Modules name, ModuleInterface *module);
    ModuleInterface* GetModule(Modules name);
    EventHandler& getEventHandler();
    void Start();
    void Terminate(bool wait);
    void WaitEnd();
};

#endif // ENGINECORE_H
