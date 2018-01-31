#ifndef SERVICE
#define SERVICE

#include <memory>
#include "../worker.h"

class Service
{
public:
    virtual ~Service() = default;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void Restart() = 0;
    virtual void Wait() = 0;
    virtual void WaitForStart()
    {

    };
};

class ServiceContainer
{
    bool isEmpty = true;
protected:
    std::shared_ptr<Service> service;
public:
    ServiceContainer(Service* service);
    ServiceContainer() = default;
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Restart();
    void Wait();
    operator bool();
    Service& GetService();
    bool operator==(const ServiceContainer& rhs);
};

template<class T>
class TypedServiceContainer: public ServiceContainer
{
public:
    TypedServiceContainer()
    {
        static_assert(std::is_base_of<Service, T>::value, "T is not a Service");
        service = std::shared_ptr<Service>(new T{});
    }

    TypedServiceContainer(const TypedServiceContainer<T>& src)
    {
        service = src.service;
    }

    T* operator->()
    {
        return static_cast<T*>(service.get());
    }
};

#endif // SERVICE

