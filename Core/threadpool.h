//#ifndef THREADPOOL_H
//#define THREADPOOL_H

//#include <thread>
//#include <list>
//#include <vector>
//#include "worker.h"

//struct ThreadPoolParams
//{
//    int minThreads;
//    int maxThreads;
//    int reserveFactor;
//};

//class ThreadSubset;

//class ThreadPool
//{
//    std::mutex mutex;
//    std::vector<Worker> workers;
//    ThreadPoolParams params;
//    std::list<Worker*> freeWorkers;
//    std::list<Worker*> reservedWorkers;
//    TaskList* poolTaskList;
//    ThreadPool(const ThreadPool& pool) = delete;
//    void releaseSubset(std::vector<Workers*>& subset);
//public:
//    ~ThreadPool();
//    ThreadPool(const ThreadPoolParams& params);
//    ThreadSubset getSubset(int minThreads, int maxThreads);
//    void Execute(Invokable& action);
//    friend class ThreadSubset;
//};

//class ThreadSubset
//{
//    std::vector<Worker*> workers;
//    ThreadPool& parentPool;
//    TaskList* poolTaskList = nullptr;
//    ThreadSubset(std::vector<Worker*>&& source, ThreadPool& parent);
//    ThreadSubset(const ThreadSubset& subset) = delete;
//public:
//    ThreadSubset(ThreadSubset&& subset);
//    ~ThreadSubset();
//    void Release();
//    void Execute(Invokable& action);
//};

//#endif // THREADPOOL_H
