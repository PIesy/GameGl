#include "inputmodule.h"

InputApi* InputModule::getApi()
{
    return api->operator ->();
}

InputModule::~InputModule()
{
    delete api;
}

void InputModule::setApi(ApiBase* api)
{
    ModuleApi<InputApi>* p = new ModuleApi<InputApi>((InputApi*)api);
    api->AttachEngine(engine);
    this->api = p;
}
