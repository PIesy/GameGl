#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <string>
#include <utility>

#define LOG_ENABLED


enum class LogMessageSeverity {Error, Warning, Info};

struct LogQueue
{
    std::mutex mutex;
    std::condition_variable newString;
    std::queue<std::pair<std::string, LogMessageSeverity>> logQueue;
};

class Logger
{
#ifdef LOG_ENABLED
    static Logger logger;
#endif
    const LogMessageSeverity lowestSeverity = LogMessageSeverity::Info;
    bool forwardToConsole = true;
    bool terminate = false;
    LogQueue queue;
    std::fstream logfile;
    std::thread logThread;
    Logger();
    ~Logger();
    void write(const std::string& str);
    void log(const std::string& str, LogMessageSeverity severity);
    void logLoop();
public:
    static void Log(const std::string& str, LogMessageSeverity severity = LogMessageSeverity::Info)
    {
    #ifdef LOG_ENABLED
        logger.queue.mutex.lock();
        logger.queue.logQueue.push({str, severity});
        logger.queue.mutex.unlock();
        logger.queue.newString.notify_all();
    #endif
    }

    static void LogError(const std::string& str)
    {
    #ifdef LOG_ENABLED
        logger.queue.mutex.lock();
        logger.queue.logQueue.push({str, LogMessageSeverity::Error});
        logger.queue.mutex.unlock();
        logger.queue.newString.notify_all();
    #endif
    }

    static void LogWarning(const std::string& str)
    {
    #ifdef LOG_ENABLED
        logger.queue.mutex.lock();
        logger.queue.logQueue.push({str, LogMessageSeverity::Warning});
        logger.queue.mutex.unlock();
        logger.queue.newString.notify_all();
    #endif
    }

    static void LogInfo(const std::string& str)
    {
    #ifdef LOG_ENABLED
        logger.queue.mutex.lock();
        logger.queue.logQueue.push({str, LogMessageSeverity::Info});
        logger.queue.mutex.unlock();
        logger.queue.newString.notify_all();
    #endif
    }
};

#endif
