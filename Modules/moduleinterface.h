#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

template<class T>
class ModuleApi
{
    T* api;
public:
    ModuleApi(T* api){ this->api = api;}
    T* operator->(){return api;}
};

class IModule
{
public:
    virtual ~IModule(){}
    virtual void setData(void* data) = 0;
    virtual void setApi(void* api) = 0;
    virtual void* getApi() = 0;
};

#include "../Core/engineinterface.h"

#endif // MODULEINTERFACE_H
