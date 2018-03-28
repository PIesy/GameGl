#include "worker.h"
#include "../Logger/logger.h"
#include "enginecore.h"
#include <sstream>

void workerController(std::shared_ptr<WorkerData> data);
void logStatus(const std::string& message);

const auto logger = Logger::GetLogger(getClassName<Worker>());

Worker::Worker()
{
    if (core::core.IsInit())
        data->core = &core::core.Get();
    workerThread = std::make_unique<std::thread>(workerController, data);
    workerId = workerThread->get_id();
}

Worker::Worker(const std::string& name)
{
    if (core::core.IsInit())
        data->core = &core::core.Get();
    data->name = name;
    workerThread = std::make_unique<std::thread>(workerController, data);
    workerId = workerThread->get_id();
}

Worker::Worker(const std::string& name, SharedTaskList& taskList)
{
    if (core::core.IsInit())
        data->core = &core::core.Get();
    data->name = name;
    data->taskList = taskList;
    workerThread = std::make_unique<std::thread>(workerController, data);
    workerId = workerThread->get_id();
}

Worker::Worker(Worker&& arg) noexcept
{
    workerId = arg.workerId;
    workerThread = std::move(arg.workerThread);
    data = std::move(arg.data);
}

Worker::~Worker()
{
    if (workerThread == nullptr)
        return;
    if(workerThread->joinable())
    {
        Terminate();
        workerThread->detach();
    }
}

bool Worker::IsBusy()
{
    return data->busy;
}

void Worker::Join()
{
    try
    {
        if(workerThread->joinable())
            workerThread->join();
    }
    catch (std::system_error&){}
}

void Worker::Terminate()
{
    data->isRunning = false;
    data->taskList.Add(Task{[this]
    {
       data->terminate = true;
    }});
}

bool Worker::Execute(const Invokable& invokable)
{
    if (data->terminate)
        return false;
    data->taskList.Add(invokable);
    return true;
}

bool Worker::Execute(Invokable&& invokable)
{
    if (data->terminate)
        return false;
    data->taskList.Add(invokable);
    return true;
}

bool Worker::IsValid()
{
    return data->isRunning;
}

void workerController(std::shared_ptr<WorkerData> data)
{
    if (data->core)
        core::core.Init(*data->core);
    data->isRunning = true;
    logStatus("Thread \"" + data->name + "\" started id:");

    try
    {
        std::shared_ptr<Invokable> task;
        while(!(data->terminate && data->taskList.IsEmpty()))
        {
            task = data->taskList.GetAndRemove();
            data->busy = true;
            task->Invoke();
            data->busy = false;
        }
    }
    catch (std::exception& e)
    {
        logger.LogError(std::string("Error ") + e.what() + " | thread name: " + data->name);
    }

    data->isRunning = false;
    logStatus("Thread \"" + data->name + "\" stopped id:");
}

void logStatus(const std::string& message)
{
    std::stringstream str;
    str << message + " " << std::this_thread::get_id();
    logger.LogDebug(str.str());
}
