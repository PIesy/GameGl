#include "service.h"


ServiceContainer::ServiceContainer(Service *service)
{
    this->service = std::shared_ptr<Service>(service);
    isEmpty = false;
}

void ServiceContainer::Start()
{
    service->Start();
}

void ServiceContainer::Stop()
{
    service->Stop();
}

void ServiceContainer::Pause()
{
    service->Pause();
}

void ServiceContainer::Resume()
{
    service->Resume();
}

void ServiceContainer::Restart()
{
    service->Restart();
}

void ServiceContainer::Wait()
{
    service->Wait();
}

ServiceContainer::operator bool()
{
    return !isEmpty;
}
