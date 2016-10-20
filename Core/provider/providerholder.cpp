#include "providerholder.h"

void ProviderHolder::addProvider(int type, Provider provider)
{
    if (providers.count(type))
    {
        providers.at(type).insert({provider.getMeta(), provider});
        return;
    }
    std::unordered_map<MetaInfo, Provider> data;
    data.insert({provider.getMeta(), provider});
    providers.insert({type, data});
}
