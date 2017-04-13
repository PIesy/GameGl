#ifndef HELPERS_H
#define HELPERS_H

#include <type_traits>
#include <typeindex>
#include "action.h"
#include "task.h"
#include "generic_invokable.h"
#include "invokablepack.h"

template<typename T>
constexpr std::underlying_type_t<T> integral(T value)
{
    return static_cast<std::underlying_type_t<T>>(value);
}

template<typename T>
constexpr std::underlying_type_t<T>* integralPointer(T* value)
{
    return reinterpret_cast<std::underlying_type_t<T>*>(value);
}

template<typename T>
constexpr const std::underlying_type_t<T>* integralPointer(const T* value)
{
    return reinterpret_cast<const std::underlying_type_t<T>*>(value);
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
constexpr GenericInvokable* generilizeInvokable(Action<ArgPtr> action)
{
    return new GenericInvokable(action);
}

inline void waitIf(const bool& condition, std::mutex& mutex, std::condition_variable& var)
{
    if(condition)
    {
        std::unique_lock<std::mutex> lock(mutex);
        while(condition)
            var.wait(lock);
    }
}

#endif // HELPERS_H
