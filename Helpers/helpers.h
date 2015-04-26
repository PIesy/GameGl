#ifndef HELPERS_H
#define HELPERS_H

#include <type_traits>
#include <typeindex>
#include "action.h"
#include "task.h"
#include "generic_invokable.h"

template<typename T>
constexpr typename std::underlying_type<T>::type integral(T value)
{
    return static_cast<typename std::underlying_type<T>::type>(value);
}

template<typename T>
constexpr std::type_index getType()
{
    return std::type_index(typeid(T));
}

template<typename T>
constexpr std::type_index getType(T& value)
{
    return std::type_index(typeid(value));
}

template<class... Args, class F, class... AllArgs>
constexpr Action<Args...> getAction(F&& value, AllArgs&&... args)
{
    return Action<Args...>(value, std::forward<AllArgs>(args)...);
}

template<class ArgPtr>
constexpr GenericInvokable generilizeInvokable(Action<ArgPtr> action)
{
    return new GenericInvokable(action);
}

inline void waitIf(bool condition, std::mutex& mutex, std::condition_variable& var)
{
    if(condition)
    {
        std::unique_lock<std::mutex> lock(mutex);
        if(condition)
            var.wait(lock);
    }
}

#endif // HELPERS_H
