#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#include "enginedatastructures.h"
#include "eventshandler_new.h"

class ModuleInterface;

class EngineInterface
{
public:
    virtual ~EngineInterface(){}
    virtual void AttachModule (Modules name, ModuleInterface* module) = 0;
    virtual ModuleInterface* GetModule (Modules name) = 0;
    virtual EventHandler& getEventHandler() = 0;
    virtual void Start() = 0;
    virtual void Terminate() = 0;
    virtual void WaitEnd() = 0;
};

#endif // ENGINEINTERFACE_H
