#include "service_cluster.h"

ServiceCluster::~ServiceCluster()
{
    for (ServiceContainer& service: services)
        service.Stop();
    mainService.Stop();
    for (ServiceContainer& service: services)
        service.Wait();
    mainService.Wait();
}

void ServiceCluster::Start()
{
    started = true;
    for (ServiceContainer& service: services)
        service.Start();
    mainService.Start();
}

void ServiceCluster::Stop()
{
    for (ServiceContainer& service: services)
        service.Stop();
    mainService.Stop();
}

void ServiceCluster::Restart()
{
    for (ServiceContainer& service: services)
        service.Restart();
    mainService.Restart();
}

void ServiceCluster::Pause()
{
    for (ServiceContainer& service: services)
        service.Pause();
    mainService.Pause();
}

void ServiceCluster::Resume()
{
    for (ServiceContainer& service: services)
        service.Resume();
    mainService.Resume();
}

void ServiceCluster::Wait()
{
    for (ServiceContainer& service: services)
        service.Wait();
    mainService.Wait();
}

void ServiceCluster::Add(ServiceContainer service)
{
    services.push_back(service);
    if (started)
        service.Start();
}

void ServiceCluster::Remove(ServiceContainer service)
{
    services.remove(service);
}

void ServiceCluster::setMainService(ServiceContainer service)
{
    mainService = service;
}
