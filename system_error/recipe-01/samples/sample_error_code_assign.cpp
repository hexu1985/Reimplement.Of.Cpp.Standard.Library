// error_code::assign
#include <iostream>       // std::cout
#include <cerrno>         // errno
#include <cmath>          // std::pow
#include "system_error.hpp"   // std::error_code, std::generic_category

struct expnumber {
  double value;
  mini_stl::error_code error;
  expnumber (double base, double exponent) {
    value = std::pow(base,exponent);
    if (errno) error.assign (errno,mini_stl::generic_category());
  }
};

int main()
{
  expnumber foo (3.0, 2.0);
  std::cout << foo.value << "\t" << foo.error.message() << '\n';

  expnumber bar (3.0, 10e6);
  std::cout << bar.value << "\t" << bar.error.message() << '\n';

  return 0;
}


/*
Possible output:

9       No error
inf     Result too large
*/

