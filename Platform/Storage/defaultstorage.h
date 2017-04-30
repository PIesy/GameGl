//
// Created by akann on 4/23/17.
//

#ifndef GAMEGL_DEFAULTSTORAGE_H
#define GAMEGL_DEFAULTSTORAGE_H

#include "../../Storage/storageapi.h"
#include "storageservice.h"
#include <unordered_map>

class DefaultStorage : public StorageApi
{
    TypedServiceContainer<StorageService> service;
public:
    StorageDescriptor Allocate(size_t size, unsigned int itemCount, unsigned int alignment) override;

    StorageDescriptor Place(size_t size, const void* data) override;

    void Erase(StorageDescriptor& descriptor) override;

    ServiceContainer getService() override;
};


#endif //GAMEGL_DEFAULTSTORAGE_H
