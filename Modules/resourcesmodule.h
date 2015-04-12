#ifndef RESOURCESMODULE_H
#define RESOURCESMODULE_H

#include "module.h"
#include "Resources/resourcesapi.h"

class ResourcesModule: public Module
{
    ModuleApi<ResourcesApi>* api = nullptr;
public:
    const void* getApi();
    ~ResourcesModule();
    void setApi(void* api);
};

#endif // RESOURCESMODULE_H
