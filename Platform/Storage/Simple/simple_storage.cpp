//
// Created by akann on 4/23/17.
//

#include "simple_storage.h"

StorageDescriptor SimpleStorage::Allocate(size_t size, unsigned int itemCount, unsigned int alignment)
{
    return service->Allocate(size, itemCount, alignment);
}

StorageDescriptor SimpleStorage::Place(size_t size, const void* data, size_t id)
{
    return service->Place(size, data, id);
}

void SimpleStorage::Erase(size_t id)
{

}

StorageDescriptor SimpleStorage::Place(size_t size, const void* data)
{
    return service->Place(size, data);
}

StorageDescriptor SimpleStorage::Get(size_t id)
{
    return service->Get(id);
}

SimpleStorage::SimpleStorage(TypedServiceContainer<StorageService>& serviceContainer) : service(serviceContainer)
{

}

SimpleStorage::SimpleStorage(SimpleStorage&& src) : service(src.service)
{

}
