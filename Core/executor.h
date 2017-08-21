#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "invokable.h"

class Executor
{
public:
    virtual ~Executor() {};
    virtual void Execute(const Invokable& invokable) = 0;
    virtual void Execute(Invokable&& invokable) = 0;
    virtual bool IsValid() = 0;
};

#endif // EXECUTOR_H

