//
// Created by akann on 8/11/17.
//

#ifndef GAMEGL_SDL2INPUTPROVIDER_H
#define GAMEGL_SDL2INPUTPROVIDER_H


#include "../../../Core/module_provider.h"

class SDL2InputProvider: public ModuleProvider
{
public:
    std::unique_ptr<ModuleInterface> GetModule() override;
};


#endif //GAMEGL_SDL2INPUTPROVIDER_H
