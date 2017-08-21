//
// Created by akann on 8/11/17.
//

#ifndef GAMEGL_MODULE_PROVIDER_H
#define GAMEGL_MODULE_PROVIDER_H

#include "../Modules/moduleinterface.h"
#include <memory>

class ModuleProvider
{
public:
    virtual ~ModuleProvider() {}
    virtual std::unique_ptr<ModuleInterface> GetModule() = 0;
};

#endif //GAMEGL_MODULE_PROVIDER_H
