#include "threadpool.h"

bool ThreadPool::Execute(const Invokable& invokable)
{
    if (!isRunning)
        return false;
    if (allBusy() && workers.size() < config.maxThreads)
        workers.emplace_back("ThreadPoolThread" + std::to_string(workers.size()), threadPoolList);
    threadPoolList.Add(invokable);
    return true;
}

bool ThreadPool::Execute(Invokable&& invokable)
{
    if (!isRunning)
        return false;
    if (allBusy() && workers.size() < config.maxThreads)
        workers.emplace_back("ThreadPoolThread" + std::to_string(workers.size()), threadPoolList);
    threadPoolList.Add(invokable);
    return true;
}

ThreadPool::ThreadPool(const ThreadPoolConfig& config) : config(config)
{
    for (int i = 0; i < this->config.minThreads; i++)
        workers.emplace_back("ThreadPoolThread" + std::to_string(i), threadPoolList);
}

bool ThreadPool::allBusy()
{
    bool result = true;

    for (Worker& worker : workers)
        result = result && worker.IsBusy();
    return result;
}

bool ThreadPool::IsValid()
{
    return isRunning;
}

ThreadPool::~ThreadPool()
{
    isRunning = false;
}
