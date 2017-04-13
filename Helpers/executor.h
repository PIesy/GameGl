#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "invokable.h"

class Executor
{
public:
    virtual void Execute(Invokable& invokable) = 0;
};

#endif // EXECUTOR_H

