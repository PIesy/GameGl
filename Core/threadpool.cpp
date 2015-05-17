//#include "threadpool.h"

//ThreadPool::ThreadPool(const ThreadPoolParams& params)
//{
//    this->params = params;
//    for (int i = 0; i < minThreads; i++)
//        workers.emplace_back();
//    freeWorkers.insert(freeWorkers.begin(), workers.begin(), workers.end());
//}

//ThreadSubset ThreadPool::getSubset(int minThreads, int maxThreads)
//{
//    std::lock_guard<std::mutex> lock(mutex);
//    std::vector<Worker*> workers;
//    int count = 0;

//    count = freeWorkers.size() < maxThreads ? freeWorkers.size() : maxThreads;
//    workers.insert(workers.begin(), freeWorkers.begin(), std::advance(freeWorkers.begin(), count));
//    reservedWorkers.insert(reservedWorkers.begin(), freeWorkers.begin(), std::advance(freeWorkers.begin(), count));
//    freeWorkers.erase(freeWorkers.begin(), std::advance(freeWorkers.begin(), count));
//    return ThreadSubset(workers);
//}

//void ThreadPool::Execute(Invokable& action)
//{
//    poolTaskList->emplace_back(action.Copy());
//}

//void ThreadPool::releaseSubset(std::vector<Workers*>& subset)
//{
//    std::lock_guard<std::mutex> lock(mutex);
//    reservedWorkers.erase(reservedWorkers.begin(), std::advance(reservedWorkers.begin(), count));
//}

//ThreadSubset::ThreadSubset(std::vector<Worker*>&& source, ThreadPool& parent):parentPool(parent)
//{
//    workers = source;
//}

//void ThreadSubset::Release()
//{
//    parentPool.releaseSubset(workers);
//}
