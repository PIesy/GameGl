#ifndef ACTION_H
#define ACTION_H

#include "invokable.h"
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
    template<class F, class... AllArgs>
    Action(F&& f, AllArgs&&... args);
    template<class F, class... AllArgs>
    void Bind(F&& f, AllArgs&&... args);
    template<class F, class... AllArgs>
    auto FutureBind(F&& f, AllArgs&&... args) -> std::future<typename function_traits<decltype(f)>::result_type>;
    Action<Args...>* Copy() const;
    void Invoke() {}
    void InvokeWithArgs(Args&&... args);
    void operator()();
};

template<class... Args>
template<class F, class... AllArgs>
Action<Args...>::Action(F&& f, AllArgs&&... args)
{
    Bind(f, std::forward<AllArgs>(args)...);
}

template<class... Args>
template<class F, class... AllArgs>
void Action<Args...>::Bind(F&& f, AllArgs&&... args)
{
    auto bound = std::bind(f, std::forward<AllArgs>(args)...);
    fun = TypedActionObject([bound](Args&&... args) { bound(std::forward<Args>(args)...); });
    isValid = true;
}

template<class... Args>
template<class F, class... AllArgs>
auto Action<Args...>::FutureBind(F&& f, AllArgs&&... args) -> std::future<typename function_traits<decltype(f)>::result_type>
{
    using ResultType = typename function_traits<decltype(f)>::result_type;
    auto package = std::make_shared<std::packaged_task<ResultType(Args...)>>(std::bind(f, std::forward<AllArgs>(args)...));
    std::future<ResultType> result = package->get_future();
    fun = TypedActionObject([package](Args&&... args) { (*package)(std::forward<Args>(args)...); });
    isValid = true;
    hasFuture = true;
    return result;
}

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

#endif // ACTION_H

