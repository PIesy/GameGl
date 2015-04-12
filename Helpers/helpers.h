#ifndef HELPERS_H
#define HELPERS_H

#include <type_traits>
#include <typeindex>

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

#endif // HELPERS_H
