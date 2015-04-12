#include "coreinterface.h"
#include "../Core/enginecore.h"
#include "../Graphics/openglsdl.h"

EngineInitializer makeInit();

CoreInterface::CoreInterface()
{
    EngineInitializer i = makeInit();
    core = new EngineCore(i);
    initVideo();
}

CoreInterface::~CoreInterface()
{
    delete core;
}

void CoreInterface::BindAction(ActionOld action, int eventType, int eventCategory)
{
    core->BindAction(action, eventType, eventCategory);
}

GraphicsApi* CoreInterface::Video()
{
    return video;
}

void CoreInterface::Start()
{
    core->Start();
}

void CoreInterface::Stop()
{
    core->Terminate();
}

int CoreInterface::WaitEnd()
{
    core->WaitEnd();
    return 1;
}

void CoreInterface::initVideo()
{
    GraphicsModule* graphics = (GraphicsModule*)core->GetModule(Modules::Video);
    video = (GraphicsApi*)graphics->getApi();
}

EngineInterface* CoreInterface::getCore()
{
    return core;
}

EngineInitializer makeInit()
{
    EngineInitializer i;
    ModuleApiPair pair;
    pair.api = new OpenGlSdl;
    pair.module = Modules::Video;
    i.apis.push_back(pair);
    return i;
}
