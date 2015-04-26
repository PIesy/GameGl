#ifndef FUNCTION_TRAITS_H
#define FUNCTION_TRAITS_H

#include <functional>

template<class F>
struct function_traits: public function_traits<decltype(&F::operator())>
{};

template<class R, class... Args>
struct function_traits<R(*&&)(Args...)> : public function_traits<R(Args...)>
{};

template<class R, class... Args>
struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)>
{};

template<class R, class... Args>
struct function_traits<R(&)(Args...)> : public function_traits<R(Args...)>
{};

template<class R, class... Args>
struct function_traits<std::function<R(Args...)>> : public function_traits<R(Args...)>
{};

template<class C, class R, class... Args>
struct function_traits<R(C::*)(Args...)> : public function_traits<R(C&,Args...)>
{};

template<class C, class R, class... Args>
struct function_traits<R(C::*)(Args...) const> : public function_traits<R(C&,Args...)>
{};

template<typename R, typename ...Args>
struct function_traits<R(Args...)>
{
    static const size_t nargs = sizeof...(Args);

    using result_type = R;

    template <size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    };
};

#endif // FUNCTION_TRAITS_H

