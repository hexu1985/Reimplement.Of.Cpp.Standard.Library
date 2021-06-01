// forward_list::begin example
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  mini_stl::forward_list<int> mylist = { 34, 77, 16, 2 };

  std::cout << "mylist contains:";
  for ( auto it = mylist.begin(); it != mylist.end(); ++it )
    std::cout << ' ' << *it;

  std::cout << '\n';

  return 0;
}


/*
Output:

mylist contains: 34 77 16 2
*/

