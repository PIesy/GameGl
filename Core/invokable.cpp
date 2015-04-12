#include "invokable.h"
#include <iostream>

void Action::invoke(void *arg)
{
    fun(arg);
}

void Action::operator()(void* arg)
{
    invoke(arg);
}
