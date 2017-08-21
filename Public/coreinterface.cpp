#include "coreinterface.h"
#include "../Core/engineinitializer.h"
#include "../Core/enginecore.h"
#include "../Platform/Graphics/OpenGL/opengl_module_provider.h"
#include "../Platform/Input/SDL2/sdl2_input_provider.h"
#include "../Platform/Storage/Simple/simple_storage_provider.h"

EngineInitializer makeInit();

CoreInterface::CoreInterface()
{
    EngineInitializer i = makeInit();
    core = new EngineCore(i);
    initVideo();
    initStorage();
}

CoreInterface::~CoreInterface()
{
    delete core;
}

EventHandler& CoreInterface::getEventHandler()
{
    return core->GetEventHandler();
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
    ModuleInterface& graphics = core->GetModule(ModuleType::Video);
    video = dynamic_cast<GraphicsApi*>(&graphics.GetApi());
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
    storage = dynamic_cast<StorageApi*>(&core->GetModule(ModuleType::Storage).GetApi());
}

EngineInitializer makeInit()
{
    EngineInitializer i;
    i.providers.emplace_back(std::shared_ptr<ModuleProvider>{new OpenGLModuleProvider()});
    i.providers.emplace_back(std::shared_ptr<ModuleProvider>{new SDL2InputProvider()});
    i.providers.emplace_back(std::shared_ptr<ModuleProvider>{new SimpleStorageProvider()});
    return i;
}
