#ifndef ENGINEINITIALIZER_H
#define ENGINEINITIALIZER_H

#include "enginedatastructures.h"
#include "../Api/apibase.h"
#include <vector>

struct ModuleApiPair
{
    ApiBase* api;
    Modules module;
};

struct EngineInitializer
{
    std::vector<ModuleApiPair> apis;
};

#endif // ENGINEINITIALIZER_H
