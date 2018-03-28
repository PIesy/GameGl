#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "invokable.h"

class Executor
{
public:
    virtual ~Executor() = default;
    virtual bool Execute(const Invokable& invokable) = 0;
    virtual bool Execute(Invokable&& invokable) = 0;
    virtual bool IsValid() = 0;
};

#endif // EXECUTOR_H

