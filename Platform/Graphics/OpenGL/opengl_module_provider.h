//
// Created by akann on 8/11/17.
//

#ifndef GAMEGL_OPENGLMODULEPROVIDER_H
#define GAMEGL_OPENGLMODULEPROVIDER_H

#include "../../../Core/module_provider.h"

class OpenGLModuleProvider: public ModuleProvider
{
public:
    std::unique_ptr<ModuleInterface> GetModule() override;
};


#endif //GAMEGL_OPENGLMODULEPROVIDER_H
