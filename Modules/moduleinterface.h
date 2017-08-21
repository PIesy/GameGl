#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include "../Core/engineinterface.h"
#include "../Core/apibase.h"
#include "../Core/meta.h"

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
    MetaInfo metaInfo;
    ModuleType type;
public:
    explicit ModuleInterface(const MetaInfo& metaInfo, ModuleType type) : metaInfo(metaInfo), type(type) {}
    virtual ~ModuleInterface(){}
    virtual ApiBase& GetApi() = 0;
    virtual ServiceContainer GetServices() = 0;

    ModuleType GetType() const
    {
        return type;
    };

    MetaInfo& GetMetaInfo()
    {
        return metaInfo;
    }


    const MetaInfo& GetMetaInfo() const
    {
        return metaInfo;
    }
};

#endif // MODULEINTERFACE_H
