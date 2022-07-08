#pragma once

#include <tuple>
#include <type_traits>

namespace Hx {

template <std::size_t... Ints> struct index_sequence {};

template <std::size_t N, std::size_t... Ints>
struct make_index_sequence_impl: make_index_sequence_impl<N - 1, N - 1, Ints...> {};

template <std::size_t... Ints>
struct make_index_sequence_impl<0u, Ints...> { 
    using type = index_sequence<Ints...>; 
};

template <std::size_t N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;

template <int I>
struct placeholder_t {
};

namespace placeholders {

placeholder_t<1> _1; placeholder_t<2> _2; placeholder_t<3> _3; placeholder_t<4> _4; placeholder_t<5> _5; 
placeholder_t<6> _6; placeholder_t<7> _7;placeholder_t<8> _8; placeholder_t<9> _9; placeholder_t<10> _10;

}   // namespace placeholders

template <typename F>
struct result_traits;

/* check function */
template <typename R, typename... P>
struct result_traits<R(*)(P...)> { typedef R type; };

/* check member function */
template <typename R, typename C, typename... P> 
struct result_traits<R(C::*)(P...)> { typedef R type; };

template <typename T, typename Tuple>
inline auto select(T&& val, Tuple&) -> T&&
{
    return std::forward<T>(val);
}

template <int I, typename Tuple>
inline auto select(placeholder_t<I>&, Tuple& tp) -> decltype(std::get<I - 1>(tp))
{
    return std::get<I - 1>(tp);
}

namespace detail {

template <typename R, typename C, typename MT, typename T1, typename... Args>
R invoke_memptr(MT C::* memptr, T1&& t1, Args&&... args)
{
    if constexpr (std::is_function_v<MT>) {
        if constexpr (std::is_base_of_v<C, std::decay_t<T1>>) {
            return (std::forward<T1>(t1).*memptr)(std::forward<Args>(args)...);
        } else {
            return (std::forward<T1>(t1)->*memptr)(std::forward<Args>(args)...);
        }
    } else {
        static_assert(std::is_object_v<MT> && sizeof...(args) == 0);
        if constexpr (std::is_base_of_v<C, std::decay_t<T1>>) {
            return std::forward<T1>(t1).*memptr;
        } else {
            return std::forward<T1>(t1)->*memptr;
        }
    }
}

template <typename R, typename F, typename... Args>
R invoke(F&& f, Args&&... args)
{
    if constexpr (std::is_member_pointer_v<std::decay_t<F>>) {
        return invoke_memptr<R>(std::forward<F>(f), std::forward<Args>(args)...);
    } else {
        return std::forward<F>(f)(std::forward<Args>(args)...);
    }
}

}   // namespace detail

template <typename Fun, typename... Args>
struct bind_t {
    typedef typename std::decay<Fun>::type FunType;
    typedef std::tuple<typename std::decay<Args>::type...> ArgType;
    typedef typename result_traits<FunType>::type ResultType;

public:
    template <typename F, typename... BArgs>
    bind_t(F&& f, BArgs&&... args) : func_(std::forward<F>(f)), args_(std::forward<BArgs>(args)...)
    {
    }

    template <typename... CArgs>
    ResultType operator()(CArgs&&... args)
    {
        return do_call(make_index_sequence<std::tuple_size<ArgType>::value>{},
                std::forward_as_tuple(std::forward<CArgs>(args)...));
    }

    template <typename ArgTuple, std::size_t... Indexes>
    ResultType do_call(index_sequence<Indexes...>&&, ArgTuple&& argtp)
    {
        return detail::invoke<ResultType>(func_, select(std::get<Indexes>(args_), argtp)...);
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
