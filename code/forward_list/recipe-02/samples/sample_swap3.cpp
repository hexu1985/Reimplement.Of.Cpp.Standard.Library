// swap lists
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  Hx::forward_list<int> first (3,100);   // three ints with a value of 100
  Hx::forward_list<int> second (5,200);  // five ints with a value of 200

  first.swap(second);

  std::cout << "first contains:";
  for (Hx::forward_list<int>::iterator it=first.begin(); it!=first.end(); it++)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "second contains:";
  for (Hx::forward_list<int>::iterator it=second.begin(); it!=second.end(); it++)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


/*
Output:

first contains: 200 200 200 200 200 
second contains: 100 100 100 


Complexity
*/
