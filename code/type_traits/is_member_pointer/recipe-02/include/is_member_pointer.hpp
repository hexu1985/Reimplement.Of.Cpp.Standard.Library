#pragma once

#include "integral_constant.hpp"
#include "remove_cv.hpp"

namespace Hx {

template< class T >
struct is_member_pointer_helper         : false_type {};
 
template< class T, class U >
struct is_member_pointer_helper<T U::*> : true_type {};
 
template< class T >
struct is_member_pointer : is_member_pointer_helper<remove_cv_t<T>> {};

}   // namespace Hx
