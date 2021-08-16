// set::equal_elements
#include <iostream>
#include "set.hpp"

int main ()
{
  mini_stl::set<int> myset;

  for (int i=1; i<=5; i++) myset.insert(i*10);   // myset: 10 20 30 40 50

  std::pair<mini_stl::set<int>::const_iterator,mini_stl::set<int>::const_iterator> ret;
  ret = myset.equal_range(30);

  std::cout << "the lower bound points to: " << *ret.first << '\n';
  std::cout << "the upper bound points to: " << *ret.second << '\n';

  return 0;
}

/*
the lower bound points to: 30
the upper bound points to: 40
*/
