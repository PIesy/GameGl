#include "threadpool.h"

void ThreadPool::Execute(const Invokable& invokable)
{
    if (allBusy())
        workers.emplace_back("ThreadPoolThread" + std::to_string(workers.size()), threadPoolList);
    threadPoolList.Add(invokable);
}

void ThreadPool::Execute(Invokable&& invokable)
{
    if (allBusy())
        workers.emplace_back("ThreadPoolThread" + std::to_string(workers.size()), threadPoolList);
    threadPoolList.Add(invokable);
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
