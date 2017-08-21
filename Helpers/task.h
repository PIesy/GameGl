#ifndef TASK_H
#define TASK_H

#include "../Core/invokable.h"
#include <future>
#include "sharedstate.h"


class Task: public Invokable
{
    std::function<void()> task;
    bool isValid = false;
    bool hasFuture = false;
    SharedState<bool> isInvoked = false;
public:
    Task() {}
    explicit Task(std::function<void()>&& f);
    explicit Task(const std::function<void()>& f);
    Task& operator=(std::function<void()>&& f);
    Task& operator=(const std::function<void()>& f);

    template<class F>
    auto SetTask(F&& f) -> std::future<typename std::result_of<F()>::type>;

    template<class F>
    auto SetTask(std::function<F()>&& f) -> std::future<typename std::result_of<F()>::type>;

    template<class F, class... Args>
    auto SetTask(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    Task* Copy() const;
    void Invoke();
    void operator()();
    void WaitTillFinished();
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

template<class F>
auto Task::SetTask(std::function<F()>&& f) -> std::future<typename std::result_of<F()>::type>
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

#endif // TASK_H

