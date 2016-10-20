#ifndef CONFIGURABLE
#define CONFIGURABLE

#include "config.h"
#include "provider/providerholder.h"

class Configurable
{
    std::type_index type = typeid(*this);
public:
    Configurable(const ProviderHolder& holder);
    virtual ~Configurable() {}
    virtual void Configure(const Config& config) = 0;
};

#endif // CONFIGURABLE

