#ifndef HELPERS_H
#define HELPERS_H

#include <type_traits>

template <typename T>
constexpr typename std::underlying_type<T>::type integral(T value)
{
    return static_cast<typename std::underlying_type<T>::type>(value);
}
#endif // HELPERS_H
