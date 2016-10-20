#ifndef DEFAULTCONFIGPROVIDER_H
#define DEFAULTCONFIGPROVIDER_H

#include "configprovider.h"
#include <unordered_map>

class DefaultConfigProvider: ConfigProvider
{
    MetaInfo meta;
    std::unordered_map<std::type_index, Config> configs;
public:
    DefaultConfigProvider(MetaInfo meta);
    MetaInfo getMeta();
    bool isReady();
    Config getConfig(std::type_index classId);
    void addConfig(const Config& config, std::type_index classId);
};

#endif // DEFAULTCONFIGPROVIDER_H
