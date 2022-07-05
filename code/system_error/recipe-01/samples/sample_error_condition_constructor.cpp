// error_condition::error_condition
#include <iostream>       // std::cout, std::ios
#include "system_error.hpp"   // std::error_condition, std::generic_category

int main() {
  std::cout << "The default error condition: ";
  std::cout << Hx::error_condition().message() << '\n';

  std::cout << "A condition constructed from errc: ";
  std::cout << Hx::error_condition(Hx::errc::permission_denied).message() << '\n';

  std::cout << "Some generic error conditions, by value:\n";
  for (int i=0; i<10; ++i) {
    Hx::error_condition c (i,Hx::generic_category());
    std::cout << "\t#" << i << ": " << c.message() << '\n';
  }
  return 0;
}


/*
Possible output:

The default error condition: No error
A condition constructed from errc: Permission denied
Some generic error conditions, by value:
        #0: No error
        #1: Operation not permitted
        #2: No such file or directory
        #3: No such process
        #4: Interrupted function call
        #5: Input/output error
        #6: No such device or address
        #7: Arg list too long
        #8: Exec format error
        #9: Bad file descriptor
*/
