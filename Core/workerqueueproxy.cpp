#include "workerqueueproxy.h"


WorkerQueueProxy::WorkerQueueProxy(TaskList* source)
{
    if(!source)
        tasks = new TaskList();
    else
    {
        externalSource = true;
        tasks = source;
    }
}

WorkerQueueProxy::WorkerQueueProxy(WorkerQueueProxy&& arg)
{
    this->tasks = arg.tasks;
    arg.tasks = nullptr;
    externalSource = arg.externalSource;
}

WorkerQueueProxy::~WorkerQueueProxy()
{
    if(!externalSource)
        delete tasks;
}

void WorkerQueueProxy::ReplaceTaskSource(TaskList& source)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(!externalSource)
        delete tasks;
    tasks = &source;
}

TaskData WorkerQueueProxy::Pop()
{
    std::lock_guard<std::mutex> lock(mutex);
    TaskData result = tasks->front();

    tasks->pop_front();
    return result;
}

void WorkerQueueProxy::Push(const Invokable& data)
{
    std::lock_guard<std::mutex> lock(mutex);

    tasks->emplace_back(data.Copy());
}

bool WorkerQueueProxy::IsEmpty()
{
    std::lock_guard<std::mutex> lock(mutex);

    return tasks->empty();
}
