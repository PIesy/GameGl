#include "logger.h"
#include <iostream>
#include <ctime>
#include "../Helpers/helpers.h"

Logger Logger::logger;

std::string handleError(const std::string& message);
std::string handleWarning(const std::string& message);
std::string handleInfo(const std::string& message);
std::string getTimestamp();

Logger::Logger() : logThread(&Logger::logLoop, this)
{
}

Logger::~Logger()
{
    terminate = true;
    queue.newString.notify_all();
    logThread.join();
}

void Logger::write(const std::string& str)
{
    if(logfile.is_open())
        logfile << str << "\n";
    if(forwardToConsole)
        std::cerr << str << "\n";
}

void Logger::log(const std::string& str, LogMessageSeverity severity)
{
    if (integral(severity) > integral(lowestSeverity))
        return;
    std::string message;

    switch (severity)
    {
        case LogMessageSeverity::Error:
            message = handleError(str);
            break;
        case LogMessageSeverity::Warning:
            message = handleWarning(str);
            break;
        case LogMessageSeverity::Info:
            message = handleInfo(str);
            break;
    }
    write(getTimestamp() + message);
}

std::string handleError(const std::string& message)
{
    return "[ERROR] " + message;
}

std::string handleWarning(const std::string& message)
{
    return "[WARNING] " + message;
}

std::string handleInfo(const std::string& message)
{
    return "[INFO] " + message;
}

std::string getTimestamp()
{
    time_t rawTime;
    struct tm* formattedTime;
    char buff[21];

    std::time(&rawTime);
    formattedTime = std::localtime(&rawTime);
    std::strftime(buff, 21, "[%x-%X]", formattedTime);
    return std::string(buff);
}

void Logger::logLoop()
{
    std::unique_lock<std::mutex> lock(queue.mutex, std::defer_lock);
    logfile.open("log.txt", std::ios_base::out | std::ios_base::trunc);

    log("Logger started", LogMessageSeverity::Info);
    while(!terminate)
    {
        if(!queue.logQueue.empty())
        {
            lock.lock();
            while (!queue.logQueue.empty())
            {
                auto& pair = queue.logQueue.front();
                log(pair.first, pair.second);
                queue.logQueue.pop();
            }
            lock.unlock();
        }
        else
        {
            lock.lock();
            while(queue.logQueue.empty() && !terminate)
                queue.newString.wait(lock);
            lock.unlock();
        }
    }

    log("Logger stopped", LogMessageSeverity::Info);
}
