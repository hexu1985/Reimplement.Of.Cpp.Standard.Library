// errc example
#include <iostream>       // std::cout
#include <system_error>   // std::error_condition, std::errc, std::generic_category

int main()
{
  // two ways of creating the same error_condition:
  std::error_condition foo (std::errc::file_exists);
  std::error_condition bar (EEXIST, std::generic_category() );
//  std::error_code cow (EEXIST, std::system_category() );

  if (foo == bar) std::cout << foo.message() << "\n";
//  if (cow == foo) std::cout << cow.message() << "\n";

  return 0;
}


/*
Possible output:

File exists
*/
