#ifndef SERVICE
#define SERVICE

#include "Core/worker.h"

class Service
{
public:
    virtual ~Service() {}
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void Restart() = 0;
    virtual void Wait() = 0;
};

class ServiceContainer
{
    std::shared_ptr<Service> service;
    bool isEmpty = true;
public:
    ServiceContainer(Service* service);
    ServiceContainer() {}
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Restart();
    void Wait();
    operator bool();
    bool operator==(const ServiceContainer& rhs);
};

#endif // SERVICE

