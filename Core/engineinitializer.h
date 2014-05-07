#ifndef ENGINEINITIALIZER_H
#define ENGINEINITIALIZER_H

#include "enginedatastructures.h"
#include <vector>

struct ModuleApiPair
{
    void* api;
    Modules module;
};

struct EngineInitializer
{
    std::vector<ModuleApiPair> apis;
};

#endif // ENGINEINITIALIZER_H
