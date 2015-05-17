#ifndef SERVICECLUSTER_H
#define SERVICECLUSTER_H

#include "service.h"

class ServiceCluster: public Service
{
    std::list<Service*> services;
public:
    ServiceCluster();
    ~ServiceCluster();
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Restart();
    void Wait();
    void Add(Service* service);
    void Remove(Service* service);
};

#endif // SERVICECLUSTER_H
