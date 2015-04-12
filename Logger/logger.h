#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <ctime>
#include <string>

#define LOG_ENABLED

struct LogQueue
{
    std::mutex mutex;
    std::condition_variable newString;
    std::queue<std::string> logQueue;
};

class Logger
{
    bool forwardToConsole = true;
    bool terminate = false;
    LogQueue queue;
    std::fstream logfile;
    std::thread* logThread;
    Logger();
    ~Logger();
    void write(std::string str);
    void log(std::string str);
public:
    static void Log(std::string str)
    {
    #ifdef LOG_ENABLED
        static Logger log;
        log.queue.logQueue.push(str);
        log.queue.mutex.lock();
        log.queue.newString.notify_all();
        log.queue.mutex.unlock();
    #endif
    }

    friend class LoggerToken;
    friend void loggerRoutine(Logger* log, LogQueue* queue);
};

#endif
