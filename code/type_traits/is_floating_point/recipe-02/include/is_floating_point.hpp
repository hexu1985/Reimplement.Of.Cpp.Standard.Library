#pragma once

#include "integral_constant.hpp"
#include "is_same.hpp"
#include "remove_cv.hpp"

namespace mini_stl {

template< class T >
struct is_floating_point
     : integral_constant<
         bool,
         is_same<float, typename remove_cv<T>::type>::value  ||
         is_same<double, typename remove_cv<T>::type>::value  ||
         is_same<long double, typename remove_cv<T>::type>::value
     > {};

}   // namespace mini_stl
