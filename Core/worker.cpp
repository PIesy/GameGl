#include "worker.h"

void SetupData(void(*f)(void*), void* arg, WorkerData *data);
void workerController(WorkerData *data);

Worker::Worker(void (*f)(void*), void* arg)
{
    SetupData(f, arg, &data);

    workerThread = new std::thread(workerController, &data);
    workerId = workerThread->get_id();
}

Worker::~Worker()
{
    if(workerThread->joinable())
    {
        workerThread->detach();
        workerThread->~thread();
    }
}

void Worker::setWork(void (*f)(void*), void* arg)
{
    if(!workerThread)
    {
        SetupData(f, arg, &data);

        workerThread = new std::thread(workerController, &data);
        workerId = workerThread->get_id();
    }
}

void Worker::Join()
{
    workerThread->join();
}

void Worker::Terminate()
{
    data.terminate = true;
    data.hasWork.notify_all();
}

void Worker::setTask(void (*f)(void*), void* arg)
{
    TaskData* t = new TaskData;

    t->fun = f;
    t->arg = arg;
    data.tasks.push(*t);
    data.hasWork.notify_all();
}

void workerController(WorkerData *data)
{
    TaskData task;
    std::unique_lock<std::mutex> lock(data->mutex);

    while(!data->terminate)
    {
        if(data->tasks.empty())
        {
            data->hasWork.wait(lock);
        }
        task = data->tasks.front();
        data->tasks.pop();
        task.fun(task.arg);
    }
}

void SetupData(void (*f)(void *), void *arg, WorkerData* data)
{
    TaskData* t = new TaskData;

    if(f)
    {
        t->fun = f;
        t->arg = arg;
        data->tasks.push(*t);
    }
}
