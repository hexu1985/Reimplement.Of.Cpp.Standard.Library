#pragma once

#include <type_traits>
#include <tuple>
#include <utility>

namespace Hx {

template <typename T, typename Tuple, std::size_t... I>
T make_from_tuple(Tuple &&t, std::index_sequence<I...>)
{
    return T(std::get<I>(std::forward<Tuple>(t))...);
}

template <class T, class Tuple>
T make_from_tuple(Tuple&& t)
{
    return make_from_tuple<T>(std::forward<Tuple>(t),
        std::make_index_sequence<std::tuple_size<Tuple>::value>{});
}

}   // namespace Hx

