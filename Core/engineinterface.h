#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#include "enginedatastructures.h"
#include "events/eventshandler.h"

class ModuleInterface;

class EngineInterface
{
public:
    virtual ~EngineInterface(){}
    virtual void AttachModule (ModuleType name, ModuleInterface* module) = 0;
    virtual ModuleInterface& GetModule (ModuleType name) = 0;
    virtual EventHandler& GetEventHandler() = 0;
    virtual Executor& GetExecutor(bool exclusive = false, const std::string& name = "") = 0;
    virtual void Start() = 0;
    virtual void Terminate() = 0;
    virtual void WaitEnd() = 0;
};

#endif // ENGINEINTERFACE_H
