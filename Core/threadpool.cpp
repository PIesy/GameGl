#include "threadpool.h"


ThreadPool::ThreadPool(int minThreads, int maxThreads)
{
    workers = new Worker[minThreads];
    params.minThreads = minThreads;
    params.maxThreads = maxThreads;
}


