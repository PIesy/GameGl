//
// Created by akann on 8/11/17.
//

#ifndef GAMEGL_SIMPLE_STORAGE_PROVIDER_H
#define GAMEGL_SIMPLE_STORAGE_PROVIDER_H


#include "../../../Core/module_provider.h"

class SimpleStorageProvider: public ModuleProvider
{
public:
    std::unique_ptr<ModuleInterface> GetModule() override;
};


#endif //GAMEGL_SIMPLE_STORAGE_PROVIDER_H
