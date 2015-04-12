#ifndef APIBASE
#define APIBASE

#include "Core/engineinterface.h"

class ApiBase
{
protected:
    EngineInterface* engine;
public:
    virtual ~ApiBase() {}
    void AttachEngine(EngineInterface* engine) { this->engine = engine; }
    virtual Service* getService() {}
};

#endif // APIBASE

