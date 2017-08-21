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
    void Start() override;
    void Stop() override;
    void Pause() override;
    void Resume() override;
    void Restart() override;
    void Wait() override;
    void SetMainService(ServiceContainer service);
    Service& GetMainService();
    void Add(ServiceContainer service);
    void Remove(ServiceContainer service);
};

#endif // SERVICECLUSTER_H
