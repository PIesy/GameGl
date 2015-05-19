#include "logger.h"
#include <iostream>

void loggerRoutine(Logger* log, LogQueue* queue);

Logger::Logger()
{
    logfile.open("log.txt", std::ios_base::out | std::ios_base::trunc);
    logThread = new std::thread(loggerRoutine, this, &this->queue);
}

Logger::~Logger()
{
    terminate = true;
    queue.mutex.lock();
    queue.newString.notify_all();
    queue.mutex.unlock();
    logThread->join();
    if(logfile.is_open())
        logfile.close();
    delete logThread;
}

void Logger::write(std::string str)
{
    if(logfile.is_open())
        logfile << str << std::endl;
    if(forwardToConsole)
        std::cerr << str << std::endl;
}

void Logger::log(std::string str)
{
    time_t rawTime;
    struct tm* formattedTime;
    char buff[200];

    time(&rawTime);
    formattedTime = localtime(&rawTime);
    strftime(buff, 200, "[%x-%X]", formattedTime);
    write(buff + str);
}

void loggerRoutine(Logger* log, LogQueue* queue)
{
    std::unique_lock<std::mutex> lock(queue->mutex, std::defer_lock);

    log->log("Logger started");
    while(!log->terminate)
    {
        while(!queue->logQueue.empty())
        {
            log->log(queue->logQueue.front());
            queue->logQueue.pop();
        }
        lock.lock();
        queue->newString.wait(lock);
        lock.unlock();
    }
    log->log("Logger stopped");
}
