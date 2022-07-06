#pragma once

#include <utility>
#include <tuple>
#include <type_traits>
#include <functional>

namespace Hx {

template <int I>
struct placeholder_t {
};

namespace placeholders {

placeholder_t<1> _1; placeholder_t<2> _2; placeholder_t<3> _3; placeholder_t<4> _4; placeholder_t<5> _5; 
placeholder_t<6> _6; placeholder_t<7> _7;placeholder_t<8> _8; placeholder_t<9> _9; placeholder_t<10> _10;

}   // namespace placeholders

template<typename F>
struct result_traits;

/* check function */
template <typename R, typename... P>
struct result_traits<R(*)(P...)> { typedef R type; };

/* check member function */
template <typename R, typename C, typename... P> 
struct result_traits<R(C::*)(P...)> { typedef R type; };

template <typename T, class Tuple>
inline auto select(T&& val, Tuple&)->T&&
{
    return std::forward<T>(val);
}

template <int I, class Tuple>
inline auto select(placeholder_t<I>&, Tuple& tp) -> decltype(std::get<I - 1>(tp))
{
    return std::get<I - 1>(tp);
}

template <typename Fun, typename... Args>
struct bind_t {
    typedef typename std::decay<Fun>::type FunType;
    typedef std::tuple<typename std::decay<Args>::type...> ArgType;
    typedef typename result_traits<FunType>::type ResultType;

public:
    template <class F, class... BArgs>
    bind_t(F&& f, BArgs&&... args) : func_(std::forward<F>(f)), args_(std::forward<BArgs>(args)...)
    {
    }

    template <typename... CArgs>
    ResultType operator()(CArgs&&... args)
    {
        return do_call(std::make_index_sequence<std::tuple_size<ArgType>::value>{},
                std::forward_as_tuple(std::forward<CArgs>(args)...));
    }

    template <typename ArgTuple, std::size_t... Indexes>
    ResultType do_call(std::index_sequence<Indexes...>&&, ArgTuple&& argtp)
    {
        return std::invoke(func_, select(std::get<Indexes>(args_), argtp)...);
    }

private:
    FunType func_;
    ArgType args_;
};

template <typename F, typename... P>
inline bind_t<F, P...> bind(F&& f, P&&... par)
{
    return bind_t<F, P...>(std::forward<F>(f), std::forward<P>(par)...);
}

}   // namespace Hx
