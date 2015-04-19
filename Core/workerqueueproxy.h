#ifndef WORKERQUEUEPROXY
#define WORKERQUEUEPROXY

#include <list>
#include <mutex>
#include "actionpack.h"
#include <memory>

struct TaskData;

using TaskList = std::list<TaskData>;

struct TaskData
{
    ActionPack fun;
    void* arg;
};

class WorkerQueueProxy
{
    std::mutex mutex;
    TaskList* tasks;
    bool externalSource = false;
public:
    WorkerQueueProxy(TaskList* source = nullptr);
    ~WorkerQueueProxy();
    WorkerQueueProxy(const WorkerQueueProxy& arg) = delete;
    WorkerQueueProxy(WorkerQueueProxy&& arg);
    void ReplaceTaskSource(TaskList* source);
    TaskData Pop();
    void Push(const TaskData& data);
    bool IsEmpty();
};

#endif // WORKERQUEUEPROXY

