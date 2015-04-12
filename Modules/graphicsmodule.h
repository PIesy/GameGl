#ifndef GRAPHICSMODULE_H
#define GRAPHICSMODULE_H

#include "module.h"
#include "Graphics/graphicsapi.h"

class GraphicsModule: public Module
{
    ModuleApi<GraphicsApi>* api = nullptr;
public:
    const void* getApi();
    ~GraphicsModule();
    void setApi(void* api);
};

#endif // GRAPHICSMODULE_H
