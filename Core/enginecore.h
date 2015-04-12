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
    std::unordered_map<int, ModuleInterface*> modules;
    CoreData* data;
    EventsHandler handler;
    void initModules();
    void initApis(EngineInitializer initializer);
    void startCoreThread();
public:
    EngineCore(EngineInitializer initializer);
    ~EngineCore();
    void AttachModule(Modules name, ModuleInterface *module);
    ModuleInterface* GetModule(Modules name);
    void BindAction(ActionOld action, int type, int category);
    void Start();
    void Terminate();
    void WaitEnd();
};

#endif // ENGINECORE_H
