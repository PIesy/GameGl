//
// Created by akann on 4/23/17.
//

#ifndef GAMEGL_STORAGEPOINTER_H
#define GAMEGL_STORAGEPOINTER_H

#include <functional>

class StoragePointer
{
    using Accessor = std::function<void*()>;
    using Validator = std::function<bool()>;
    static const Accessor defaultAccessor;
    static const Validator defaultValidator;

    Accessor accessor;
    Validator validator;
public:
    StoragePointer(const Accessor& accessor = defaultAccessor, const Validator& validator = defaultValidator)
    {
        this->accessor = accessor;
        this->validator = validator;
    }

    void* GetPointer() const
    {
        return accessor();
    }

    bool IsValid() const
    {
        return validator();
    }

    operator void*() const
    {
        return accessor();
    }
};

template<class T>
class TypedStoragePointer: public StoragePointer
{
public:
    TypedStoragePointer() {}
    TypedStoragePointer(const StoragePointer& src):StoragePointer(src){}

    T* Access() const
    {
        return static_cast<T*>(GetPointer());
    }

    operator T*() const
    {
        return Access();
    }

    T& operator[](int i) const
    {
        return Access()[i];
    }
};


#endif //GAMEGL_STORAGEPOINTER_H
