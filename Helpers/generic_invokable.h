#ifndef GENERIC_IVOKABLE_H
#define GENERIC_IVOKABLE_H
#include "invokable.h"
#include "type_traits"
#include <functional>
#include <memory>

class GenericInvokable: public TypedInvokable<void*>
{
    std::function<void(void*)> action;
    bool isValid = false;
public:
    GenericInvokable() {}
    template<class T>
    GenericInvokable(const TypedInvokable<T>& action);
    virtual GenericInvokable* Copy() const;
    virtual void Invoke() {}
    virtual void InvokeWithArgs(void*&& args);
    virtual void operator()() {}
};

template<class T>
GenericInvokable::GenericInvokable(const TypedInvokable<T>& action)
{
    if(std::is_pointer<T>::value)
    {
        auto sharedAction = std::shared_ptr<TypedInvokable<T>>(action.Copy());
        this->action = [sharedAction](void* arg) { sharedAction->InvokeWithArgs((T)arg); };
        isValid = true;
    }
}

#endif // GENERIC_IVOKABLE_H

