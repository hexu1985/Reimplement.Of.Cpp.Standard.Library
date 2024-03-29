// forward_list::empty
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  Hx::forward_list<int> first;
  Hx::forward_list<int> second = {20, 40, 80};
  std::cout << "first " << (first.empty() ? "is empty" : "is not empty" ) << std::endl;
  std::cout << "second " << (second.empty() ? "is empty" : "is not empty" ) << std::endl;
  return 0;
}


/*
Output:

first is empty
second is not empty
*/


