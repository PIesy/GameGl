#ifndef SERVICECLUSTER_H
#define SERVICECLUSTER_H

#include "service.h"

class ServiceCluster: public Service
{
    ServiceContainer mainService;
    std::list<ServiceContainer> services;
    bool started = false;
public:
    ~ServiceCluster();
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Restart();
    void Wait();
    void setMainService(ServiceContainer service);
    void Add(ServiceContainer service);
    void Remove(ServiceContainer service);
};

#endif // SERVICECLUSTER_H
