#include "logger.h"
#include <iostream>
#include <ctime>
#include "../Helpers/helpers.h"

#ifdef LOG_ENABLED
Logger Logger::logger;
#endif

std::string handleError();
std::string handleWarning();
std::string handleInfo();
std::string handleDebug();
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

void Logger::log(const std::string& str, const std::string& className, LogMessageSeverity severity)
{
    if (integral(severity) > integral(lowestSeverity))
        return;
    std::string message;

    switch (severity)
    {
        case LogMessageSeverity::Error:
            message += handleError();
            break;
        case LogMessageSeverity::Warning:
            message += handleWarning();
            break;
        case LogMessageSeverity::Info:
            message += handleInfo();
            break;
        case LogMessageSeverity::Debug:
            message += handleDebug();
    }
    message += "[" + className + "] " + str;
    write(getTimestamp() + message);
}

std::string handleError()
{
    return "[ERROR]";
}

std::string handleWarning()
{
    return "[WARNING]";
}

std::string handleInfo()
{
    return "[INFO]";
}

std::string handleDebug()
{
    return "[DEBUG]";
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

    log("Logger started", getClassName<Logger>(),LogMessageSeverity::Info);
    while(!terminate)
    {
        if(!queue.logQueue.empty())
        {
            lock.lock();
            while (!queue.logQueue.empty())
            {
                auto& item = queue.logQueue.front();
                log(item.message, item.className, item.severity);
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

    log("Logger stopped", getClassName<Logger>(), LogMessageSeverity::Info);
}
