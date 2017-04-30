#ifndef COREINTERFACE_H
#define COREINTERFACE_H

#include "apis.h"
#include "../Core/engineinterface.h"
#include "../Graphics/renderdefs.h"
#include "../Storage/storageapi.h"

class CoreInterface
{
    EngineInterface* core;
    GraphicsApi* video;
    ResourcesApi* resources;
    StorageApi* storage;
    void initVideo();
    void initResources();
    void initStorage();
public:
    CoreInterface();
    ~CoreInterface();
    GraphicsApi* Video();
    ResourcesApi& Resources();
    EngineInterface* getCore();
    EventHandler& getEventHandler();
    StorageApi& GetStorage();
    void Start();
    void Stop();
    int WaitEnd();
};

#endif // COREINTERFACE_H
