#ifndef RESOURCESMODULE_H
#define RESOURCESMODULE_H

#include "moduleinterface.h"
#include "Resources/resourcesapi.h"

class ResourcesModule: public IModule
{
    ModuleApi<ResourcesApi>* api;
    const EngineInterface* core;
public:
    void* getApi();
    ~ResourcesModule();
    void setApi(void* api);
    void setData(void* data);
};

#endif // RESOURCESMODULE_H
