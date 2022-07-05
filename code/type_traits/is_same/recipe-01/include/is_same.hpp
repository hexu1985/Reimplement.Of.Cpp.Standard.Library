#pragma once

#include <type_traits>

namespace mini_stl {

template <typename T, typename U>
struct is_same : std::false_type {};
 
template <typename T>
struct is_same<T, T> : std::true_type {};

template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

}   // namespace mini_stl
