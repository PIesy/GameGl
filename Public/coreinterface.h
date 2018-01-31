#ifndef COREINTERFACE_H
#define COREINTERFACE_H

#include "apis.h"
#include "../Core/engineinterface.h"
#include "../Storage/storageapi.h"

class CoreInterface
{
    EngineInterface* core;
    GraphicsApi* video;
    StorageApi* storage;
    void initVideo();
    void initStorage();
public:
    CoreInterface();
    ~CoreInterface();
    GraphicsApi* Video();
    EngineInterface* getCore();
    EventHandler& getEventHandler();
    StorageApi& GetStorage();
    void Start();
    void Stop();
    int WaitEnd();
};

#endif // COREINTERFACE_H
