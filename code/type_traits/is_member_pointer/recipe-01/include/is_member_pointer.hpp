#pragma once

#include <type_traits>

namespace Hx {

template <typename T>
struct is_member_pointer_helper         : std::false_type {};
 
template <typename T, typename U>
struct is_member_pointer_helper<T U::*> : std::true_type {};
 
template <typename T>
struct is_member_pointer : 
    is_member_pointer_helper<typename std::remove_cv<T>::type> {};

template <typename T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

}   // namespace Hx
