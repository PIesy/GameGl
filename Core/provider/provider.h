#ifndef PROVIDER_H
#define PROVIDER_H

#include "Core/meta.h"
#include <memory>

class ProviderInterface
{
public:
    virtual ~ProviderInterface() {}
    virtual MetaInfo getMeta() = 0;
    virtual bool isReady() = 0;
};


class Provider: public ProviderInterface
{
    std::shared_ptr<ProviderInterface> provider;
public:
    Provider(ProviderInterface* provider);
    MetaInfo getMeta();
    bool isReady();
};

#endif // PROVIDER_H

