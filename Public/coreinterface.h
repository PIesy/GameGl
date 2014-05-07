#ifndef COREINTERFACE_H
#define COREINTERFACE_H

#include "apis.h"
#include "../Core/engineinterface.h"
#include "../Graphics/renderdefs.h"

class CoreInterface
{
    EngineInterface* core;
    GraphicsApi* video;
    void initVideo();
public:
    CoreInterface();
    ~CoreInterface();
    GraphicsApi* Video();
    EngineInterface* getCore();
    void BindAction(Action action, int eventType, int eventCategory);
    void Start();
    void Stop();
    int WaitEnd();
};

#endif // COREINTERFACE_H
