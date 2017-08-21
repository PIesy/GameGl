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

template<class R, class... Args>
constexpr Action<Args...> getAction(std::function<R(Args...)> fun)
{
    return Action<Args...>(fun);
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

template <typename T>
inline constexpr int signum(T x, std::false_type is_signed) {
    return T(0) < x;
}

template <typename T>
inline constexpr int signum(T x, std::true_type is_signed) {
    return (T(0) < x) - (x < T(0));
}

template <typename T>
inline constexpr int signum(T x) {
    return signum(x, std::is_signed<T>());
}

template<typename T>
std::string getClassName()
{
    std::type_index typeIndex = typeid(T);
    return typeIndex.name();
}

#endif // HELPERS_H
