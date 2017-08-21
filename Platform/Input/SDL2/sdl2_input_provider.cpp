//
// Created by akann on 8/11/17.
//

#include "sdl2_input_provider.h"
#include "sdlinput.h"
#include "../../../Modules/module.h"
#include "../../../Core/enginecore.h"
#include "../../../Core/service/service_cluster.h"

std::unique_ptr<ModuleInterface> SDL2InputProvider::GetModule()
{
    MetaInfo meta{"sdl2_input", 0, 0};
    ServiceContainer container{new SDLInputService(core::core.Get().GetEventHandler())};
    ServiceCluster* cluster = new ServiceCluster();

    cluster->SetMainService(container);
    return std::make_unique<Module>(std::make_shared<SDLInput>(SDLInput{}), meta, ModuleType::Input, ServiceContainer{cluster});
}
