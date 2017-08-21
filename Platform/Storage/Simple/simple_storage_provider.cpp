//
// Created by akann on 8/11/17.
//

#include "simple_storage_provider.h"
#include "simple_storage.h"
#include "../../../Modules/module.h"
#include "../../../Core/service/service_cluster.h"

std::unique_ptr<ModuleInterface> SimpleStorageProvider::GetModule()
{
    MetaInfo meta{"simple_storage", 0, 0};
    TypedServiceContainer<StorageService> container;
    ServiceCluster* cluster = new ServiceCluster();

    cluster->SetMainService(container);
    return std::make_unique<Module>(std::make_shared<SimpleStorage>(SimpleStorage{container}), meta, ModuleType::Storage, ServiceContainer{cluster});
}
