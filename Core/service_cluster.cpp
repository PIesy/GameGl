#include "service_cluster.h"

ServiceCluster::ServiceCluster()
{

}

ServiceCluster::~ServiceCluster()
{

}

void ServiceCluster::Start()
{

}

void ServiceCluster::Stop()
{

}

void ServiceCluster::Restart()
{

}

void ServiceCluster::Pause()
{

}

void ServiceCluster::Resume()
{

}

void ServiceCluster::Wait()
{

}

void ServiceCluster::Add(Service* service)
{
    services.push_back(service);
}

void ServiceCluster::Remove(Service *service)
{
    services.remove(service);
}
