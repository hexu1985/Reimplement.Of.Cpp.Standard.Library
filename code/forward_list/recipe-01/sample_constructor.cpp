// constructing forward_lists
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  // constructors used in the same order as described above:
  Hx::forward_list<int> first;                                // empty forward_list of ints
  Hx::forward_list<int> second (4,100);                       // four ints with value 100
  Hx::forward_list<int> third (second.begin(),second.end());  // iterating through second
  Hx::forward_list<int> fourth (third);                       // a copy of third

  // the iterator constructor can also be used to construct from arrays:
  int myints[] = {16,2,77,29};
  Hx::forward_list<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

  std::cout << "The contents of fifth are: ";
  for (Hx::forward_list<int>::iterator it = fifth.begin(); it != fifth.end(); it++)
    std::cout << *it << ' ';

  std::cout << '\n';

  return 0;
}

/*
Output:

The contents of fifth are: 16 2 77 29 
*/

