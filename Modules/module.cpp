#include "module.h"

Module::Module(std::shared_ptr<ApiBase>&& api, const MetaInfo& metaInfo, ModuleType type, ServiceContainer serviceContainer)
        : ModuleInterface(metaInfo, type), api(api), serviceContainer(serviceContainer) {}

ApiBase& Module::GetApi()
{
    return *api;
}

ServiceContainer Module::GetServices()
{
    return serviceContainer;
}
