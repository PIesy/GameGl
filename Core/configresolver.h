#ifndef CONFIGRESOLVER_H
#define CONFIGRESOLVER_H

#include <unordered_map>
#include <typeindex>
#include "configurable.h"
#include "provider/providerholder.h"

class ConfigResolver
{
    static ConfigResolver resolver;
    std::unordered_map<std::type_index, std::reference_wrapper<Configurable>> configurables;
public:
    ConfigResolver();
    void Init(const ProviderHolder& providers);
    void Register(const Configurable& configurable);
    ~ConfigResolver();
};

#endif // CONFIGRESOLVER_H
