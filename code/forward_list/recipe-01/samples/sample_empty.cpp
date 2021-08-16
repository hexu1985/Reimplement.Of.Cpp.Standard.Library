// forward_list::empty
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  mini_stl::forward_list<int> mylist;
  int sum (0);

  for (int i=1;i<=10;++i) mylist.push_front(i);

  while (!mylist.empty())
  {
     sum += mylist.front();
     mylist.pop_front();
  }

  std::cout << "total: " << sum << '\n';
  
  return 0;
}

/*
The example initializes the content of the container to a sequence of numbers (form 1 to 10). It then pops the elements one by one until it is empty and calculates their sum.

Output:

total: 55
*/
