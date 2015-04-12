#include "worker.h"
#include "Logger/logger.h"
#include <sstream>

void setupData(Action fun, void* arg, WorkerData *data);
void workerController(WorkerData *data);
void logStatus(std::string message);

Worker::Worker()
{
    data = new WorkerData();
    workerThread = new std::thread(workerController, data);
    workerId = workerThread->get_id();
}

Worker::Worker(Action fun, void* arg)
{
    data = new WorkerData();
    data->tasks.push({fun, arg});
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
    {
        workerThread->detach();
        workerThread->~thread();
    }
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

void Worker::setTask(Action fun, void* arg)
{
    data->tasks.push({fun, arg});
    Wake();
}

void workerController(WorkerData *data)
{
    TaskData task;
    std::unique_lock<std::mutex> lock(data->mutex, std::defer_lock);

    logStatus("Thread started id:");
    while(!data->terminate)
    {
        while(data->tasks.empty())
        {
            lock.lock();
            data->hasWork.wait(lock);
            lock.unlock();
        }
        task = data->tasks.front();
        data->tasks.pop();
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
