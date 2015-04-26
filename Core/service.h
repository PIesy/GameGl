#ifndef SERVICE
#define SERVICE

#include "worker.h"

class Service
{
protected:
    Worker serviceThread;
public:
    virtual ~Service() {}
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    virtual void Restart() = 0;
    virtual void Wait() = 0;
};

#endif // SERVICE

