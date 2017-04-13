#ifndef RESOURCESMODULE_H
#define RESOURCESMODULE_H

#include "module.h"
#include "../Resources/resourcesapi.h"

class ResourcesModule: public Module
{
    ModuleApi<ResourcesApi>* api = nullptr;
public:
    ResourcesApi* getApi();
    ~ResourcesModule();
    void setApi(ApiBase* api);
};

#endif // RESOURCESMODULE_H
