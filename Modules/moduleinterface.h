#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include "Core/engineinterface.h"
#include "Api/apibase.h"

template<class T>
class ModuleApi
{
    T* api;
public:
    ModuleApi(T* api) { this->api = api; }
    T* operator->() { return api; }
};

class ModuleInterface
{
public:
    virtual ~ModuleInterface(){}
    virtual void setEngine(EngineInterface* engine) = 0;
    virtual void setApi(ApiBase* api) = 0;
    virtual ApiBase* getApi() = 0;
};

#endif // MODULEINTERFACE_H
