// error_code::operator bool
#include <iostream>       // std::cout
#include <cerrno>         // errno
#include <cmath>          // std::pow
#include <system_error>   // std::error_code, std::generic_category

struct expnumber {
  double value;
  std::error_code error;
  expnumber (double base, double exponent) {
    value = std::pow(base,exponent);
    if (errno) error.assign (errno,std::generic_category());
  }
};

int main()
{
  expnumber foo (3.0, 2.0), bar (3.0, 10e6);

  std::cout << "foo: ";
  if (!foo.error) std::cout << foo.value << '\n';
  else std::cout << "Error: " << foo.error.message() << '\n';

  std::cout << "bar: ";
  if (!bar.error) std::cout << bar.value << '\n';
  else std::cout << "Error: " << bar.error.message() << '\n';

  return 0;
}


/*
Possible output:

foo: 9
bar: Error: Result too large
*/
