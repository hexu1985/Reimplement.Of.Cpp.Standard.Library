#pragma once

#include "integral_constant.hpp"
#include "remove_cv.hpp"

namespace Hx {

template <typename T>
struct is_member_pointer_helper         : false_type {};
 
template <typename T, typename U>
struct is_member_pointer_helper<T U::*> : true_type {};
 
template <typename T>
struct is_member_pointer : is_member_pointer_helper<remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

}   // namespace Hx
