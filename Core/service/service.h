#ifndef SERVICE
#define SERVICE

#include <memory>
#include "../worker.h"

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
    bool isEmpty = true;
protected:
    std::shared_ptr<Service> service;
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

template<class T>
class TypedServiceContainer: public ServiceContainer
{
public:
    TypedServiceContainer()
    {
        service = std::shared_ptr<Service>(new T());
    }

    T* operator->()
    {
        return static_cast<T*>(service.get());
    }
};

#endif // SERVICE

