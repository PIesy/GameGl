//
// Created by akann on 4/23/17.
//

#ifndef GAMEGL_DEFAULTSTORAGE_H
#define GAMEGL_DEFAULTSTORAGE_H

#include "../../../Storage/storageapi.h"
#include "storageservice.h"
#include <unordered_map>

class SimpleStorage : public StorageApi
{
    TypedServiceContainer<StorageService> service;
public:
    explicit SimpleStorage(TypedServiceContainer<StorageService>& serviceContainer);
    SimpleStorage(const SimpleStorage& src) = delete;
    SimpleStorage(SimpleStorage&& src);
    StorageDescriptor Allocate(size_t size, unsigned int itemCount, unsigned int alignment) override;
    StorageDescriptor Place(size_t size, const void* data, size_t id) override;
    StorageDescriptor Place(size_t size, const void* data) override;
    StorageDescriptor Get(size_t id) override;
    void Erase(size_t id) override;
};


#endif //GAMEGL_DEFAULTSTORAGE_H
