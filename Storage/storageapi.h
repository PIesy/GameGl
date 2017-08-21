//
// Created by akann on 4/23/17.
//

#ifndef GAMEGL_STORAGE_H
#define GAMEGL_STORAGE_H

#include "../Core/apibase.h"
#include "storagedescriptor.h"

class StorageApi: public ApiBase
{
public:
    virtual ~StorageApi(){}
    virtual StorageDescriptor Allocate(size_t size, unsigned itemCount = 1, unsigned alignment = 1) = 0;
    virtual StorageDescriptor Place(size_t size, const void* data) = 0;
    virtual StorageDescriptor Place(size_t size, const void* data, size_t id) = 0;
    virtual StorageDescriptor Get(size_t id) = 0;
    virtual void Erase(size_t id) = 0;
};


#endif //GAMEGL_STORAGE_H
