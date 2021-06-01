// forward_list::before_begin
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  mini_stl::forward_list<int> mylist = {20, 30, 40, 50};

  mylist.insert_after ( mylist.before_begin(), 11 );

  std::cout << "mylist contains:";
  for ( int& x: mylist ) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}


/*
Output:

mylist contains: 11 20 30 40 50
*/

