#include "graphicsmodule.h"

void *GraphicsModule::getApi()
{
    return api->operator ->();
}

GraphicsModule::~GraphicsModule()
{
    delete api;
}

void GraphicsModule::setApi(void* api)
{
    ModuleApi<GraphicsApi>* p = new ModuleApi<GraphicsApi>((GraphicsApi*)api);
    this->api = p;
}

void GraphicsModule::setData(void *data)
{
    this->core = (const EngineInterface*)data;
}
