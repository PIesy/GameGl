#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "invokable.h"
#include "workerqueueproxy.h"

struct WorkerData
{
    std::mutex mutex;
    std::condition_variable hasWork;
    bool terminate = false;
    WorkerQueueProxy tasks;
};

class Worker
{
    std::thread::id workerId;
    WorkerData* data;
    std::thread* workerThread = nullptr;
public:
    Worker(TaskList* tasks = nullptr);
    Worker(Invokable& fun, void* arg, TaskList* tasks = nullptr);
    Worker(const Worker& arg) = delete;
    Worker(Worker&& arg);
    ~Worker();
    void setTask(Invokable& fun, void* arg);
    void Join();
    bool isBusy();
    void Wake();
    void Terminate();
};

#endif // WORKER_H
