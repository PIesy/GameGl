#include "resourcesmodule.h"

ResourcesApi* ResourcesModule::getApi()
{
    return api->operator ->();
}

ResourcesModule::~ResourcesModule()
{
    delete api;
}

void ResourcesModule::setApi(ApiBase* api)
{
    ModuleApi<ResourcesApi>* p = new ModuleApi<ResourcesApi>((ResourcesApi*)api);
    api->AttachEngine(engine);
    this->api = p;
}
