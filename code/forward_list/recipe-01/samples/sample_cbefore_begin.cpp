// forward_list::cbefore_begin
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  Hx::forward_list<int> mylist = {77, 2, 16};

  mylist.insert_after ( mylist.cbefore_begin(), 19 );

  std::cout << "mylist contains:";
  for ( int& x: mylist ) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}


/*
Output:

mylist contains: 19 77 2 16
*/

