#pragma once

#include <type_traits>

namespace Hx {

template< class T >
struct is_member_pointer_helper         : std::false_type {};
 
template< class T, class U >
struct is_member_pointer_helper<T U::*> : std::true_type {};
 
template< class T >
struct is_member_pointer : 
    is_member_pointer_helper<typename std::remove_cv<T>::type> {};

}   // namespace Hx
