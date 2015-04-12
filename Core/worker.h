#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "invokable.h"

struct TaskData
{
    Action fun;
    void* arg;
};

struct WorkerData
{
    std::mutex mutex;
    std::condition_variable hasWork;
    bool terminate = false;
    std::queue<TaskData> tasks;
};

class Worker
{
    std::thread::id workerId;
    WorkerData* data;
    std::thread* workerThread = nullptr;
public:
    Worker();
    Worker(Action fun, void* arg);
    Worker(const Worker& arg) = delete;
    Worker(Worker&& arg);
    ~Worker();
    void setTask(Action fun, void* arg);
    void Join();
    bool isBusy();
    void Wake();
    void Terminate();
};

#endif // WORKER_H
