//
// Created by akann on 4/25/17.
//

#include "storagemodule.h"

void StorageModule::setApi(ApiBase* api)
{
    ModuleApi<StorageApi>* p = new ModuleApi<StorageApi>(dynamic_cast<StorageApi*>(api));
    api->AttachEngine(*engine);
    this->api = p;
}

StorageApi* StorageModule::getApi()
{
    return api->operator->();
}

StorageModule::~StorageModule()
{
    delete api;
}
