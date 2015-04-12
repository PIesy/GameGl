#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include "Core/engineinterface.h"

template<class T>
class ModuleApi
{
    T* api;
public:
    ModuleApi(T* api){ this->api = api;}
    T* operator->(){return api;}
};

class ModuleInterface
{
public:
    virtual ~ModuleInterface(){}
    virtual void setEngine(const EngineInterface* engine) = 0;
    virtual void setApi(void* api) = 0;
    virtual const void* getApi() = 0;
};

#endif // MODULEINTERFACE_H
