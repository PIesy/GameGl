#include "graphicsmodule.h"

GraphicsApi* GraphicsModule::getApi()
{
    return api->operator ->();
}

GraphicsModule::~GraphicsModule()
{
    delete api;
}

void GraphicsModule::setApi(ApiBase* api)
{
    ModuleApi<GraphicsApi>* p = new ModuleApi<GraphicsApi>((GraphicsApi*)api);
    api->AttachEngine(engine);
    this->api = p;
}
