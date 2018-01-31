#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "invokable.h"
#include "executor.h"
#include "../Helpers/SharedTaskList.h"
#include <atomic>

class EngineInterface;

struct WorkerData
{
    EngineInterface* core;
    std::condition_variable hasWork;
    SharedTaskList taskList;
    std::string name = "";
    bool terminate = false;
    std::atomic_bool isRunning{false};
    std::atomic_bool busy{false};
};

class Worker: public Executor
{
    std::thread::id workerId;
    std::shared_ptr<WorkerData> data{new WorkerData()};
    std::unique_ptr<std::thread> workerThread;
public:
    Worker();
    explicit Worker(const std::string& name);
    Worker(const std::string& name, SharedTaskList& taskList);
    Worker(const Worker&) = delete;
    Worker(Worker&& arg);
    ~Worker();
    void Execute(const Invokable& invokable) override;
    void Execute(Invokable&& invokable) override;
    void Join();
    bool IsBusy();
    void Terminate();
    bool IsValid() override;
};

#endif // WORKER_H
