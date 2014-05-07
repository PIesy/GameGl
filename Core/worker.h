#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

struct TaskData
{
    void (*fun)(void* arg);
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
    WorkerData data;
    std::thread* workerThread = nullptr;
public:
    Worker(){}
    Worker(void (*f)(void*), void* arg);
    ~Worker();
    void setWork(void (*f)(void*), void* arg);
    void setTask(void (*f)(void*), void* arg);
    void Join();
    void getId();
    void Terminate();
};

#endif // WORKER_H
