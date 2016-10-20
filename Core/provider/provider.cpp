#include "provider.h"

Provider::Provider(ProviderInterface* provider)
{
    this->provider = std::shared_ptr<ProviderInterface>(provider);
}

MetaInfo Provider::getMeta()
{
    return provider->getMeta();
}

bool Provider::isReady()
{
    return provider->isReady();
}
