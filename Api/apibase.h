#ifndef APIBASE
#define APIBASE

#include "Core/engineinterface.h"
#include "Core/service/service.h"

class ApiBase
{
protected:
    EngineInterface* engine;
public:
    virtual ~ApiBase() {}
    void AttachEngine(EngineInterface& engine) { this->engine = &engine; }
    virtual ServiceContainer getService() { return ServiceContainer(); }
};

#endif // APIBASE

