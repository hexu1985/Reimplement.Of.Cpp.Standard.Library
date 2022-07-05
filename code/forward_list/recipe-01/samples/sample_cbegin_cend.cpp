// forward_list::cbegin/cend example
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  Hx::forward_list<int> mylist = { 21, 32, 12 };

  std::cout << "myarray contains:";
  for ( auto it = mylist.cbegin(); it != mylist.cend(); ++it )
    std::cout << ' ' << *it;   // cannot modify *it

  std::cout << '\n';

  return 0;
}


/*
Output:

mylist contains: 21 32 12
*/

