#include "worker.h"
#include "Logger/logger.h"
#include <sstream>

void workerController(WorkerData *data);
void logStatus(std::string message);

Worker::Worker(TaskList* tasks)
{
    data = new WorkerData();
    if(tasks)
        data->tasks.ReplaceTaskSource(tasks);
    workerThread = new std::thread(workerController, data);
    workerId = workerThread->get_id();
}

Worker::Worker(const ActionPack& fun, void* arg, TaskList* tasks)
{
    data = new WorkerData();
    if(tasks)
        data->tasks.ReplaceTaskSource(tasks);
    data->tasks.Push({fun, arg});
    workerThread = new std::thread(workerController, data);
    workerId = workerThread->get_id();
}

Worker::Worker(Worker&& arg)
{
    workerId = arg.workerId;
    workerThread = arg.workerThread;
    arg.workerThread = nullptr;
    data = arg.data;
    arg.data = nullptr;
}

Worker::~Worker()
{
    if(workerThread->joinable())
        workerThread->detach();
    delete workerThread;
    delete data;
}

bool Worker::isBusy()
{
    return !data->mutex.try_lock();
}

void Worker::Join()
{
    if(workerThread->joinable())
        workerThread->join();
}

void Worker::Wake()
{
    data->mutex.lock();
    data->hasWork.notify_all();
    data->mutex.unlock();
}

void Worker::Terminate()
{
    data->terminate = true;
    Wake();
}

void Worker::setTask(const ActionPack& fun, void* arg)
{
    data->tasks.Push({fun, arg});
    Wake();
}

void workerController(WorkerData *data)
{
    TaskData task;
    std::unique_lock<std::mutex> lock(data->mutex, std::defer_lock);

    logStatus("Thread started id:");
    while(!data->terminate)
    {
        while(data->tasks.IsEmpty())
        {
            lock.lock();
            if(data->tasks.IsEmpty())
                data->hasWork.wait(lock);
            lock.unlock();
        }
        task = data->tasks.Pop();
        task.fun(task.arg);
    }
    logStatus("Thread stopped id:");
}

void logStatus(std::string message)
{
    std::stringstream str;
    str << message + " " << std::this_thread::get_id();
    Logger::Log(str.str());
}
