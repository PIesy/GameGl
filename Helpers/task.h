#ifndef TASK_H
#define TASK_H

#include "invokable.h"
#include <future>


class Task: public Invokable
{
    std::function<void()> task;
    bool isValid = false;
    bool hasFuture = false;
public:
    Task() {}
    template<class F>
    Task(F&& f);
    template<class F, class... Args>
    Task(F&& f, Args&&... args);
    template<class F>
    auto SetTask(F&& f) -> std::future<typename std::result_of<F()>::type>;
    template<class F, class... Args>
    auto SetTask(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    Task* Copy() const;
    void Invoke();
    void operator()();
};

template<class F>
auto Task::SetTask(F&& f) -> std::future<typename std::result_of<F()>::type>
{
    using ResultType = typename std::result_of<F()>::type;
    auto package = std::make_shared<std::packaged_task<ResultType()>>(f);
    std::future<ResultType> result = package->get_future();
    task = std::function<void()>([package]() { (*package)(); });
    isValid = true;
    hasFuture = true;
    return result;
}

template<class F, class... Args>
auto Task::SetTask(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using ResultType = typename std::result_of<F(Args...)>::type;
    auto package = std::make_shared<std::packaged_task<ResultType()>>(std::bind(f, std::forward<Args>(args)...));
    std::future<ResultType> result = package->get_future();
    task = std::function<void()>([package]() { (*package)(); });
    isValid = true;
    hasFuture = true;
    return result;
}

template<class F, class... Args>
Task::Task(F&& f, Args&&... args)
{
    auto bound = std::bind(f, std::forward<Args>(args)...);
    task = std::function<void()>([bound]() { bound(); });
    isValid = true;
}

template<class F>
Task::Task(F&& f)
{
    task = std::function<void()>(f);
    isValid = true;
}

#endif // TASK_H

