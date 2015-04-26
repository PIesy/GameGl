#include "inputmodule.h"

const void* InputModule::getApi()
{
    return api->operator ->();
}

InputModule::~InputModule()
{
    delete api;
}

void InputModule::setApi(void* api)
{
    ModuleApi<InputApi>* p = new ModuleApi<InputApi>((InputApi*)api);
    this->api = p;
}
