#ifndef ENGINECORE_H
#define ENGINECORE_H

#include "engineinterface.h"
#include "enginecoreincludes.h"

struct CoreData
{
    EngineInterface* core;
    bool terminate = false;
};

class EngineCore: public EngineInterface
{
    std::unordered_map<int, IModule*> modules;
    CoreData* data;
    Worker worker;
    EventsHandler handler;
    void initModules();
    void initApis(EngineInitializer initializer);
    void startCoreThread();
public:
    EngineCore(EngineInitializer initializer);
    ~EngineCore();
    void AttachModule (Modules name, IModule *module);
    IModule* GetModule (Modules name);
    void BindAction(Action action, int type, int category);
    void Start();
    void Terminate();
    void WaitEnd();
};

#endif // ENGINECORE_H
