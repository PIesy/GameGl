#ifndef COREINTERFACE_H
#define COREINTERFACE_H

#include "apis.h"
#include "../Core/engineinterface.h"
#include "../Graphics/renderdefs.h"

class CoreInterface
{
    EngineInterface* core;
    GraphicsApi* video;
    ResourcesApi* resources;
    void initVideo();
    void initResources();
public:
    CoreInterface();
    ~CoreInterface();
    GraphicsApi* Video();
    ResourcesApi& Resources();
    EngineInterface* getCore();
    EventHandler& getEventHandler();
    void Start();
    void Stop();
    int WaitEnd();
};

#endif // COREINTERFACE_H
