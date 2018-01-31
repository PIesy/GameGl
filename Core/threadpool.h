#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include "executor.h"
#include "worker.h"

struct ThreadPoolConfig
{
    int minThreads;
    int maxThreads;
};

class ThreadPool: public Executor
{
    std::vector<Worker> workers;
    SharedTaskList threadPoolList;
    ThreadPoolConfig config;
    bool allBusy();
    std::atomic_bool isRunning{true};
public:
    ~ThreadPool() override;
    explicit ThreadPool(const ThreadPoolConfig& config);
    void Execute(const Invokable& invokable) override;
    void Execute(Invokable&& invokable) override;
    bool IsValid() override;
};

#endif // THREADPOOL_H
