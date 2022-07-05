#pragma once

#include <type_traits>

namespace Hx {

template <typename T>
struct is_floating_point
     : std::integral_constant<
         bool,
         std::is_same<float, typename std::remove_cv<T>::type>::value  ||
         std::is_same<double, typename std::remove_cv<T>::type>::value  ||
         std::is_same<long double, typename std::remove_cv<T>::type>::value
     > {};

}   // namespace Hx
