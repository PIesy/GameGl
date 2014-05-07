#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#include "eventshandler.h"
#include "Modules/moduleinterface.h"
#include "enginedatastructures.h"

class EngineInterface
{
public:
    virtual ~EngineInterface(){}
    virtual void AttachModule (Modules name, IModule *module) = 0;
    virtual IModule* GetModule (Modules name) = 0;
    virtual void BindAction(Action action, int type, int category) = 0;
    virtual void Start() = 0;
    virtual void Terminate() = 0;
    virtual void WaitEnd() = 0;
};

#endif // ENGINEINTERFACE_H
