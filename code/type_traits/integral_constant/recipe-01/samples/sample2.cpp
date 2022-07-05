// factorial as an integral_constant
#include <iostream>
#include "integral_constant.hpp"

template <unsigned n>
struct factorial : mini_stl::integral_constant<int,n * factorial<n-1>::value> {};

template <>
struct factorial<0> : mini_stl::integral_constant<int,1> {};

int main() {
  std::cout << factorial<5>::value;  // constexpr (no calculations on runtime)
  std::cout << std::endl;
  return 0;
}
