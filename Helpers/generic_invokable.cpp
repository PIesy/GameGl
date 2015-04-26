#include "generic_invokable.h"


void GenericInvokable::InvokeWithArgs(void*&& args)
{
    if(isValid)
        action(args);
}


GenericInvokable* GenericInvokable::Copy() const
{
    return new GenericInvokable(*this);
}
