#include "coreinterface.h"
#include "../Core/engineinitializer.h"
#include "../Modules/graphicsmodule.h"
#include "../Modules/resourcesmodule.h"
#include "../Core/enginecore.h"
#include "../Resources/filereader.h"
#include "../Input/sdlinput.h"
#include "../Platform/Graphics/OpenGL/openglsdl.h"
#include "../Platform/Storage/defaultstorage.h"

EngineInitializer makeInit();

CoreInterface::CoreInterface()
{
    EngineInitializer i = makeInit();
    core = new EngineCore(i);
    initVideo();
    initResources();
    initStorage();
}

CoreInterface::~CoreInterface()
{
    delete core;
}

EventHandler& CoreInterface::getEventHandler()
{
    return core->getEventHandler();
}

GraphicsApi* CoreInterface::Video()
{
    return video;
}

ResourcesApi& CoreInterface::Resources()
{
    return *resources;
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
    GraphicsModule& graphics = (GraphicsModule&)core->GetModule(Modules::Video);
    video = graphics.getApi();
}

void CoreInterface::initResources()
{
    ResourcesModule& resources = (ResourcesModule&)core->GetModule(Modules::Storage);
    this->resources = resources.getApi();
}

EngineInterface* CoreInterface::getCore()
{
    return core;
}

StorageApi& CoreInterface::GetStorage()
{
    return *storage;
}

void CoreInterface::initStorage()
{
    storage = dynamic_cast<StorageApi*>(core->GetModule(Modules::Memory).getApi());
}

EngineInitializer makeInit()
{
    EngineInitializer i;
    i.apis.push_back({new OpenGlSdl(), Modules::Video});
    i.apis.push_back({new SDLInput(), Modules::Input});
    i.apis.push_back({new FileReader(), Modules::Storage});
    i.apis.push_back({new DefaultStorage(), Modules::Memory});
    return i;
}
