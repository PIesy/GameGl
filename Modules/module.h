#ifndef MODULE
#define MODULE

#include "moduleinterface.h"

class Module: public ModuleInterface
{
protected:
    EngineInterface* engine;
public:
    virtual ~Module() {}
    void setEngine(EngineInterface& engine);
    virtual void setApi(ApiBase* api) = 0;
    virtual ApiBase* getApi() = 0;
};

#endif // MODULE

