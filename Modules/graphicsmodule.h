#ifndef GRAPHICSMODULE_H
#define GRAPHICSMODULE_H

#include "module.h"
#include "Graphics/graphicsapi.h"

class GraphicsModule: public Module
{
    ModuleApi<GraphicsApi>* api = nullptr;
public:
    GraphicsApi* getApi();
    ~GraphicsModule();
    void setApi(ApiBase* api);
};

#endif // GRAPHICSMODULE_H
