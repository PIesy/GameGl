#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "workerqueueproxy.h"
#include "../Helpers/invokable.h"

struct WorkerData
{
    std::mutex mutex;
    std::condition_variable hasWork;
    bool terminate = false;
    WorkerQueueProxy tasks;
    bool isRunning = false;
    std::string name = "";
};

class Worker
{
    std::thread::id workerId;
    std::shared_ptr<WorkerData> data{new WorkerData()};
    std::thread* workerThread = nullptr;
public:
    Worker();
    Worker(const std::string& name);
    Worker(TaskList& tasks);
    Worker(const Invokable& fun);
    Worker(const Invokable& fun, TaskList& tasks);
    Worker(const Worker&) = delete;
    Worker(Worker&& arg);
    ~Worker();
    void setTask(const Invokable& fun);
    void Join();
    bool isBusy();
    void Wake();
    void Terminate();
    void setName(std::string name);
};

#endif // WORKER_H
