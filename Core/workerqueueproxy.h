#ifndef WORKERQUEUEPROXY
#define WORKERQUEUEPROXY

#include <list>
#include <mutex>
#include <memory>
#include "../Helpers/invokable.h"

using TaskData = std::shared_ptr<Invokable>;

using TaskList = std::list<TaskData>;

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
    void ReplaceTaskSource(TaskList& source);
    TaskData Pop();
    void Push(const Invokable& data);
    bool IsEmpty();
};

#endif // WORKERQUEUEPROXY

