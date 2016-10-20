#ifndef CONFIGPROVIDER_H
#define CONFIGPROVIDER_H

#include <typeindex>
#include "Core/provider/provider.h"
#include "Core/config.h"

class ConfigProvider: ProviderInterface
{
public:
    virtual Config getConfig(std::type_index classId) = 0;
    virtual void addConfig(const Config& config, std::type_index classId) = 0;
};

#endif // CONFIGPROVIDER_H

