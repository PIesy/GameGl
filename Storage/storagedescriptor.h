//
// Created by akann on 4/23/17.
//

#ifndef GAMEGL_STORAGEDESCRIPTOR_H
#define GAMEGL_STORAGEDESCRIPTOR_H

#include "../Helpers/storagepointer.h"

struct StorageDescriptor
{
    StoragePointer pointer;
    size_t id = 0;
    unsigned alignment = 0;
    size_t size = 0;
    unsigned itemCount = 0;

    bool operator==(const StorageDescriptor& rhs) const
    {
        return id == rhs.id &&
               alignment == rhs.alignment &&
               size == rhs.size &&
               itemCount == rhs.itemCount;
    }

    bool operator!=(const StorageDescriptor& rhs) const
    {
        return !(rhs == *this);
    }
};

#endif //GAMEGL_STORAGEDESCRIPTOR_H
