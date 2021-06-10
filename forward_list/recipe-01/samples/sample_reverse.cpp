// reversing forward_list
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  mini_stl::forward_list<int> mylist;

  for (int i=1; i<10; ++i) mylist.push_front(i);

  mylist.reverse();

  std::cout << "mylist contains:";
  for (mini_stl::forward_list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';

  return 0;
}

/*
Output:
mylist contains: 1 2 3 4 5 6 7 8 9
*/
