#ifndef GRAPHICSMODULE_H
#define GRAPHICSMODULE_H

#include "moduleinterface.h"
#include "Graphics/graphicsapi.h"

class GraphicsModule: public IModule
{
    ModuleApi<GraphicsApi>* api;
    const EngineInterface* core;
public:
    void* getApi();
    ~GraphicsModule();
    void setApi(void* api);
    void setData(void* data);
};

#endif // GRAPHICSMODULE_H
