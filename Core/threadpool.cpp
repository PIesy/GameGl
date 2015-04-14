//#include "threadpool.h"


//ThreadPool::ThreadPool(int minThreads, int maxThreads)
//{
//    for (int i = 0; i < minThreads; i++)
//        workers.emplace_back();
//    freeWorkers.insert(freeWorkers.begin(), workers.begin(), workers.end());
//    params.minThreads = minThreads;
//    params.maxThreads = maxThreads;
//}

//ThreadSubset ThreadPool::getSubset(int minThreads, int maxThreads)
//{
//    ThreadSubset subset;
//    int count = 0;
//    std::unique_lock<std::mutex> lock(mutex);

//    subset.subsetId = currentId;
//    currentId++;
//    count = freeWorkers.size() < maxThreads ? freeWorkers.size() : maxThreads;
//    subset.workers.insert(workers.begin(), freeWorkers.begin(), freeWorkers.begin() += freeWorkers.size());
//    busyWorkers.insert(busyWorkers.begin(), freeWorkers.begin(), freeWorkers.begin() += freeWorkers.size());
//    freeWorkers.erase(freeWorkers.begin(), freeWorkers.begin() += freeWorkers.size());
//    lock.unlock();
//    subset.init();
//    return subset;
//}

//void ThreadPool::Execute(Invokable& action, void* arg)
//{
//    poolTaskList->push_back({action.copy(), arg});
//}


