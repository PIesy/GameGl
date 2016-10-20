#include "defaultconfigprovider.h"

DefaultConfigProvider::DefaultConfigProvider(MetaInfo meta)
{
    this->meta = meta;
}

MetaInfo DefaultConfigProvider::getMeta()
{
    return meta;
}

bool DefaultConfigProvider::isReady()
{
    return true;
}

Config DefaultConfigProvider::getConfig(std::type_index classId)
{
    if (configs.count(classId))
        return configs.at(classId);
    return Config();
}

void DefaultConfigProvider::addConfig(const Config& config, std::type_index classId)
{
    if (configs.count(classId))
        return;
    configs.insert({classId, config});
}
