#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include "invokable.h"
#include "worker.h"

struct ThreadPoolParams
{
    int minThreads;
    int maxThreads;
};

class ThreadSubset
{
    Worker* mainWorkers;
    int subsetId;
    ThreadSubset(Worker* mainWorker);
public:
    friend class ThreadPool;
};

class ThreadPool
{
    Worker* workers;
    ThreadPoolParams params;
    std::vector<Worker*> freeWorkers;
    std::vector<Worker*> busyWorkers;
public:
    ThreadPool(int minThreads, int maxThreads);
    ThreadSubset getSubset(int minThreads, int maxThreads);
    void ReleaseSubset(ThreadSubset subset);
    void Execute(Action action);
};

#endif // THREADPOOL_H
