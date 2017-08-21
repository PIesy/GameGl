//
// Created by akann on 8/11/17.
//

#include "opengl_module_provider.h"
#include "openglsdl.h"
#include "../../../Modules/module.h"

std::unique_ptr<ModuleInterface> OpenGLModuleProvider::GetModule()
{
    MetaInfo meta{"opengl", 0, 0};
    ServiceContainer container{new GraphicsService()};
    ServiceCluster* cluster = new ServiceCluster();

    cluster->SetMainService(container);
    return std::make_unique<Module>(std::make_shared<OpenGlSdl>(OpenGlSdl{*cluster}), meta, ModuleType::Video, ServiceContainer{cluster});
}
