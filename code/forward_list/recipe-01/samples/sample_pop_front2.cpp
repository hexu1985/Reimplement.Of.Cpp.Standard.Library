// forward_list::pop_front
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  Hx::forward_list<int> mylist = {10, 20, 30, 40};

  std::cout << "Popping out the elements in mylist:";
  while (!mylist.empty())
  {
    std::cout << ' ' << mylist.front();
    mylist.pop_front();
  }

  std::cout << '\n';

  return 0;
}

/*
Output:

Popping out the elements in mylist: 10 20 30 40
*/
