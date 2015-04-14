#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <list>
#include <vector>
#include "invokable.h"
#include "worker.h"

struct ThreadPoolParams
{
    int minThreads;
    int maxThreads;
};

class ThreadSubset;

class ThreadPool
{
    std::mutex mutex;
    int currentId = 0;
    std::vector<Worker> workers;
    ThreadPoolParams params;
    std::list<Worker*> freeWorkers;
    std::list<Worker*> busyWorkers;
    TaskList* poolTaskList;
public:
    ~ThreadPool();
    ThreadPool(int minThreads, int maxThreads);
    ThreadSubset getSubset(int minThreads, int maxThreads);
    void ReleaseSubset(ThreadSubset subset);
    void Execute(Invokable& action, void* arg);
};

class ThreadSubset
{
    std::vector<Worker*> workers;
    int subsetId;
    TaskList* poolTaskList = nullptr;
    ThreadSubset();
    void init();
public:
    ~ThreadSubset();
    void Execute(Invokable& action, void* arg);
    friend class ThreadPool;
};

#endif // THREADPOOL_H
