#include "worker.h"
#include "../Logger/logger.h"
#include <sstream>

void workerController(std::shared_ptr<WorkerData> data);
void logStatus(std::string message);

Worker::Worker()
{
    workerThread = new std::thread(workerController, data);
    workerId = workerThread->get_id();
}


Worker::Worker(const std::string& name)
{
    workerThread = new std::thread(workerController, data);
    workerId = workerThread->get_id();
    data->name = name;
}

Worker::Worker(TaskList& tasks)
{
    data->tasks.ReplaceTaskSource(tasks);
    workerThread = new std::thread(workerController, data);
    workerId = workerThread->get_id();
}

Worker::Worker(const Invokable &fun)
{
    data->tasks.Push(fun);
    workerThread = new std::thread(workerController, data);
    workerId = workerThread->get_id();
}

Worker::Worker(const Invokable& fun, TaskList& tasks)
{
    data->tasks.ReplaceTaskSource(tasks);
    data->tasks.Push(fun);
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
        Terminate();
        workerThread->detach();
    }
    delete workerThread;
}

bool Worker::isBusy()
{
    std::unique_lock<std::mutex> lock(data->mutex, std::defer_lock);
    return !lock.try_lock();
}

void Worker::Join()
{
    try {
        if(workerThread->joinable())
            workerThread->join();
    } catch (std::system_error){}
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

void Worker::setTask(const Invokable& fun)
{
    data->tasks.Push(fun);
    Wake();
}

void Worker::setName(std::string name)
{
    data->name = name;
}

void workerController(std::shared_ptr<WorkerData> data)
{
    TaskData task;
    std::unique_lock<std::mutex> lock(data->mutex, std::defer_lock);

    data->isRunning = true;
    logStatus("Thread \"" + data->name + "\" started id:");
    try
    {
        while(!data->terminate)
        {
            while(!data->tasks.IsEmpty())
            {
                task = data->tasks.Pop();
                task->Invoke();
            }
            if(data->tasks.IsEmpty())
            {
                lock.lock();
                while(data->tasks.IsEmpty() && !data->terminate)
                    data->hasWork.wait(lock);
                lock.unlock();
            }
        }
    }
    catch (std::exception e)
    {
        Logger::Log(std::string("Error ") + e.what() + " name: " + data->name);
    }

    data->isRunning = false;
    logStatus("Thread \"" + data->name + "\" stopped id:");
}

void logStatus(std::string message)
{
    std::stringstream str;
    str << message + " " << std::this_thread::get_id();
    Logger::Log(str.str());
}
