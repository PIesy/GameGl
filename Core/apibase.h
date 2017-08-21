#ifndef APIBASE
#define APIBASE

#include "engineinterface.h"
#include "service/service.h"

class ApiBase
{
protected:
    EngineInterface* engine;
public:
    virtual ~ApiBase() {}
};

#endif // APIBASE

