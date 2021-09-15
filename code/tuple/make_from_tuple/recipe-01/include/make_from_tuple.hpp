#ifndef MINI_STL_MAKE_FROM_TUPLE_INC
#define MINI_STL_MAKE_FROM_TUPLE_INC

#include <type_traits>
#include <tuple>

namespace mini_stl {

namespace detail {

template <std::size_t...> struct index_sequence {};

template <std::size_t N, std::size_t... Is>
struct make_index_sequence: make_index_sequence<N - 1, N - 1, Is...> {};

template <std::size_t... Is>
struct make_index_sequence<0u, Is...> : index_sequence<Is...> { 
    using type = index_sequence<Is...>; 
};

template <typename T, typename Tuple, std::size_t... I>
T make_from_tuple_impl(Tuple &&t, index_sequence<I...>)
{
    return T(std::get<I>(std::forward<Tuple>(t))...);
}

}    // namespace detail

template <class T, class Tuple>
T make_from_tuple(Tuple&& t)
{
    return detail::make_from_tuple_impl<T>(std::forward<Tuple>(t),
        detail::make_index_sequence<std::tuple_size<Tuple>::value>{});
}

}   // namespace mini_stl

#endif  // MINI_STL_MAKE_FROM_TUPLE_INC
