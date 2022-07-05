#pragma once

#include "integral_constant.hpp"

namespace mini_stl {

template<class T, class U>
struct is_same : false_type {};
 
template<class T>
struct is_same<T, T> : true_type {};

template< class T, class U >
inline constexpr bool is_same_v = is_same<T, U>::value;

}   // namespace mini_stl
