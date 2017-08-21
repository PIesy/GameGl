#ifndef ENGINEINITIALIZER_H
#define ENGINEINITIALIZER_H

#include "enginedatastructures.h"
#include "apibase.h"
#include "module_provider.h"
#include <vector>

struct EngineInitializer
{
    std::vector<std::shared_ptr<ModuleProvider>> providers;
};

#endif // ENGINEINITIALIZER_H
