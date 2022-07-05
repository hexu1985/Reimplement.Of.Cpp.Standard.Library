#pragma once

#include <cstddef>
#include <type_traits>

namespace Hx {

template <typename T, T... Ints> struct integer_sequence {
    static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
};

template <std::size_t... Ints> 
using index_sequence = integer_sequence<std::size_t, Ints...>;

template <typename T, T N, typename = void>
struct make_integer_sequence_helper {
    template <typename>
    struct unfold;

    template <T... Prev>
    struct unfold<integer_sequence<T, Prev...>> {
        using type = integer_sequence<T, Prev..., N-1>;
    };

    using type = typename unfold<typename make_integer_sequence_helper<T, N-1>::type>::type;
};

template <typename T, T N>
struct make_integer_sequence_helper<T, N, typename std::enable_if<N==0>::type> {
    using type = integer_sequence<T>;
};

template <typename T, T N>
using make_integer_sequence = typename make_integer_sequence_helper<T, N>::type;

template <std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

template<class... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;

}   // namespace Hx

