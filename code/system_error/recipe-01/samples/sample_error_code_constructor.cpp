// error_code constructors
#include <iostream>       // std::cout
#include <cmath>          // std::sqrt
#include <cerrno>         // errno
#include "system_error.hpp"   // std::error_code, std::generic_category
                          // std::error_condition
int main()
{
  errno=0;
  std::sqrt(-1.0);        // errno set to EDOM
  Hx::error_code ec (errno,Hx::generic_category());

  Hx::error_condition ok;
  if (ec != ok) std::cout << "Error: " << ec.message() << '\n';

  return 0;
}


/*
Possible output (cout):

Error: Domain error
*/

