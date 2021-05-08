// errc example
#include <iostream>       // std::cout
#include "system_error.hpp"   // mini_stl::error_condition, mini_stl::errc, mini_stl::generic_category

int main()
{
  // two ways of creating the same error_condition:
  mini_stl::error_condition foo (mini_stl::errc::file_exists);
  mini_stl::error_condition bar (EEXIST, mini_stl::generic_category() );
//  mini_stl::error_code cow (EEXIST, std::system_category() );

  if (foo == bar) std::cout << foo.message() << "\n";
//  if (cow == foo) std::cout << cow.message() << "\n";

  return 0;
}


/*
Possible output:

File exists
*/
