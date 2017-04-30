//
// Created by akann on 4/23/17.
//

#ifndef GAMEGL_STORAGE_H
#define GAMEGL_STORAGE_H

#include "../Api/apibase.h"
#include "storagedescriptor.h"

class StorageApi: public ApiBase
{
public:
    virtual ~StorageApi(){}
    virtual StorageDescriptor Allocate(size_t size, unsigned itemCount = 1, unsigned alignment = 1) = 0;
    virtual StorageDescriptor Place(size_t size, const void* data) = 0;
    virtual void Erase(StorageDescriptor& descriptor) = 0;
};


#endif //GAMEGL_STORAGE_H
