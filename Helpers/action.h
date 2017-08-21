#ifndef ACTION_H
#define ACTION_H

#include "../Core/invokable.h"
#include "future"
#include "function_traits.h"

template<class... Args>
class Action: public TypedInvokable<Args...>
{
    using TypedActionObject = std::function<void(Args...)>;
    TypedActionObject fun;
    bool isValid = false;
    bool hasFuture = false;
public:
    Action(){}
    Action(const std::function<void(Args...)>& fun);
    Action(std::function<void(Args...)>&& fun);

    template<typename R>
    Action(const std::function<R(Args...)>& fun);

    template<typename R>
    Action(std::function<R(Args...)>&& fun);

    template<typename R>
    std::future<R> Assign(const std::function<R(Args...)>& action);

    template<typename R>
    std::future<R> Assign(std::function<R(Args...)>&& action);

    Action<Args...>* Copy() const;
    void Invoke() {}

    void InvokeWithArgs(Args&&... args);
    void operator()();
};

template<>
void Action<>::Invoke();

template<class... Args>
void Action<Args...>::operator()()
{
    Invoke();
}

template<class... Args>
void Action<Args...>::InvokeWithArgs(Args&&... args)
{
    if(isValid)
    {
        if(hasFuture)
            isValid = false;
        fun(std::forward<Args>(args)...);
    }
}

template<class... Args>
Action<Args...>* Action<Args...>::Copy() const
{
    return new Action<Args...>(*this);
}

template<class... Args>
template<typename R>
Action<Args...>::Action(std::function<R(Args...)>&& action)
{
    fun = TypedActionObject([action](Args&&... args) { action(std::forward<Args>(args)...); });
    isValid = true;
}

template<class... Args>
template<typename R>
Action<Args...>::Action(const std::function<R(Args...)>& action)
{
    fun = TypedActionObject([action](Args&&... args) { action(std::forward<Args>(args)...); });
    isValid = true;
}

template<class... Args>
Action<Args...>::Action(std::function<void(Args...)>&& action)
{
    fun = TypedActionObject([action](Args&&... args) { action(std::forward<Args>(args)...); });
    isValid = true;
}

template<class... Args>
Action<Args...>::Action(const std::function<void(Args...)>& action)
{
    fun = TypedActionObject([action](Args&&... args) { action(std::forward<Args>(args)...); });
    isValid = true;
}

template<class... Args>
template<typename R>
std::future<R> Action<Args...>::Assign(const std::function<R(Args...)>& action)
{
    std::packaged_task<R(Args...)> package{action};
    auto future = package.get_future();
    fun = TypedActionObject([package{std::move(package)}](Args&&... args) { package(std::forward<Args>(args)...); });
    isValid = true;
    hasFuture = true;
    return future;
}

template<class... Args>
template<typename R>
std::future<R> Action<Args...>::Assign(std::function<R(Args...)>&& action)
{
    std::packaged_task<R(Args...)> package{action};
    auto future = package.get_future();
    fun = TypedActionObject([package{std::move(package)}](Args&&... args) { package(std::forward<Args>(args)...); });
    isValid = true;
    hasFuture = true;
    return future;
}

#endif // ACTION_H

