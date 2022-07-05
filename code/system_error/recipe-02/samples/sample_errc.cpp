// errc example
#include <iostream>       // std::cout
#include "system_error.hpp"   // Hx::error_condition, Hx::errc, Hx::generic_category

int main()
{
  // two ways of creating the same error_condition:
  Hx::error_condition foo (Hx::errc::file_exists);
  Hx::error_condition bar (EEXIST, Hx::generic_category() );
//  Hx::error_code cow (EEXIST, std::system_category() );

  if (foo == bar) std::cout << foo.message() << "\n";
//  if (cow == foo) std::cout << cow.message() << "\n";

  return 0;
}


/*
Possible output:

File exists
*/
