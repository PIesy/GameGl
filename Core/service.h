#ifndef SERVICE
#define SERVICE

#include "invokable.h"
#include "worker.h"

class Service
{
public:
    virtual ~Service() {}
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void WaitEnd() = 0;
};

#endif // SERVICE

