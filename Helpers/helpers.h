#ifndef HELPERS_H
#define HELPERS_H

#include <type_traits>
#include <typeindex>
#include "Core/invokable.h"

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

template<typename T, typename Callable>
constexpr Action<T> getAction(Callable value, int id = -1)
{
    return Action<T>(value, id);
}

#endif // HELPERS_H
