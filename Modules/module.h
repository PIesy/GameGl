#ifndef MODULE
#define MODULE

#include "moduleinterface.h"

class Module: public ModuleInterface
{
    ServiceContainer serviceContainer;
    std::shared_ptr<ApiBase> api;
public:
    Module(std::shared_ptr<ApiBase>&& api, const MetaInfo& metaInfo, ModuleType type, ServiceContainer serviceContainer);
    ApiBase& GetApi() override;
    ServiceContainer GetServices() override;

};

#endif // MODULE

