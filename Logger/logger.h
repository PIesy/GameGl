#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <string>
#include <utility>
#include <sstream>

#define LOG_ENABLED

enum class LogMessageSeverity {Error, Warning, Info, Debug};

struct QueueItem
{
    std::string message;
    std::string className;
    LogMessageSeverity severity;
};

struct LogQueue
{
    std::mutex mutex;
    std::condition_variable newString;
    std::queue<QueueItem> logQueue;
};

class Logger
{
    class LogProxy
    {
        Logger& logger;
        std::string className;
        LogProxy(Logger& logger, std::string className) noexcept : logger(logger), className(std::move(className)) {}
    public:
        void Log(const std::string& str, LogMessageSeverity severity) const
        {
            logger.Log(str, className, severity);
        }

        void LogDebug(const std::string& str) const
        {
            logger.Log(str, className, LogMessageSeverity::Debug);
        }

        void LogError(const std::string& str) const
        {
            logger.Log(str, className, LogMessageSeverity::Error);
        }

        void LogWarning(const std::string& str) const
        {
            logger.Log(str, className, LogMessageSeverity::Warning);
        }

        void LogInfo(const std::string& str) const
        {
            logger.Log(str, className, LogMessageSeverity::Info);
        }

        template<typename T>
        LogProxy& operator<<(const T& value)
        {
            std::stringstream stream;

            stream << value;
            LogInfo(stream.str());
            return *this;
        }
        friend class Logger;
    };

#ifdef LOG_ENABLED
    static Logger logger;
#endif
    const LogMessageSeverity lowestSeverity = LogMessageSeverity::Debug;
    bool forwardToConsole = true;
    bool terminate = false;
    LogQueue queue;
    std::fstream logfile;
    std::thread logThread;
    Logger();
    ~Logger();
    void write(const std::string& str);
    void log(const std::string& str, const std::string& className, LogMessageSeverity severity);
    void logLoop();
public:
    void Log(const std::string& str, const std::string& className, LogMessageSeverity severity)
    {
    #ifdef LOG_ENABLED
        std::unique_lock<std::mutex> lock(logger.queue.mutex);
        logger.queue.logQueue.push({str, className, severity});
        lock.unlock();
        logger.queue.newString.notify_all();
    #endif
    }

    static LogProxy GetLogger(const std::string& className) noexcept
    {
        return LogProxy{logger, className};
    }
};

#endif
