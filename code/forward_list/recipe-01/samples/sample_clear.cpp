// clearing lists
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  mini_stl::forward_list<int> mylist;
  mini_stl::forward_list<int>::iterator it;

  mylist.push_front (300);
  mylist.push_front (200);
  mylist.push_front (100);

  std::cout << "mylist contains:";
  for (it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  mylist.clear();
  mylist.push_front (2202);
  mylist.push_front (1101);

  std::cout << "mylist contains:";
  for (it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

/*
Output:
mylist contains: 100 200 300
mylist contains: 1101 2202
*/

