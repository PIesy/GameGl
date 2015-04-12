#ifndef MODULE
#define MODULE

#include "moduleinterface.h"

class Module: public ModuleInterface
{
protected:
    const EngineInterface* engine;
public:
    virtual ~Module() {}
    void setEngine(const EngineInterface* engine);
    virtual void setApi(void* api) = 0;
    virtual const void* getApi() = 0;
};

#endif // MODULE

