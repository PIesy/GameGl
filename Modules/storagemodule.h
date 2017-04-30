//
// Created by akann on 4/25/17.
//

#ifndef GAMEGL_STORAGEMODULE_H
#define GAMEGL_STORAGEMODULE_H

#include "module.h"
#include "../Storage/storageapi.h"

class StorageModule: public Module
{
    ModuleApi<StorageApi>* api = nullptr;
public:
    void setApi(ApiBase* api) override;
    ~StorageModule();
    StorageApi* getApi() override;
};


#endif //GAMEGL_STORAGEMODULE_H
