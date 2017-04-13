#ifndef PROVIDERHOLDER
#define PROVIDERHOLDER

#include "provider.h"
#include <unordered_map>
#include "../meta.h"
#include "../enginedatastructures.h"

class ProviderHolder
{
    std::unordered_map<int, std::unordered_map<MetaInfo, Provider>> providers;
public:
    void addProvider(int type, Provider provider);
};

#endif // PROVIDERHOLDER

