//
// Created by akann on 4/23/17.
//

#include "defaultstorage.h"

StorageDescriptor DefaultStorage::Allocate(size_t size, unsigned int itemCount, unsigned int alignment)
{
    return service->Allocate(size, itemCount, alignment);
}

StorageDescriptor DefaultStorage::Place(size_t size, const void* data)
{
    return service->Place(size, data);
}

void DefaultStorage::Erase(StorageDescriptor& descriptor)
{

}

ServiceContainer DefaultStorage::getService()
{
    return service;
}
