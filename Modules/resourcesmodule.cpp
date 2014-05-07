#include "resourcesmodule.h"

void* ResourcesModule::getApi()
{
    return api->operator ->();
}

ResourcesModule::~ResourcesModule()
{
    delete api;
}

void ResourcesModule::setApi(void* api)
{
    ModuleApi<ResourcesApi>* p = new ModuleApi<ResourcesApi>((ResourcesApi*)api);
    this->api = p;
}

void ResourcesModule::setData(void *data)
{
    this->core = (const EngineInterface*)data;
}
