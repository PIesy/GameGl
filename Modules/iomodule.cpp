#include "iomodule.h"

const void* IoModule::getApi()
{
    return api->operator ->();
}

IoModule::~IoModule()
{
    delete api;
}

void IoModule::setApi(void* api)
{
    ModuleApi<IoApi>* p = new ModuleApi<IoApi>((IoApi*)api);
    this->api = p;
}
