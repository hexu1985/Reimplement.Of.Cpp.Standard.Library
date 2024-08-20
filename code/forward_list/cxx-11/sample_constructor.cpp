// constructing forward_lists
#include <iostream>
#include <forward_list>

int main ()
{
  // constructors used in the same order as described above:
  std::forward_list<int> first;                                // empty forward_list of ints
  std::forward_list<int> second (4,100);                       // four ints with value 100
  std::forward_list<int> third (second.begin(),second.end());  // iterating through second
  std::forward_list<int> fourth (third);                       // a copy of third

  // the iterator constructor can also be used to construct from arrays:
  int myints[] = {16,2,77,29};
  std::forward_list<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

  std::cout << "The contents of fifth are: ";
  for (std::forward_list<int>::iterator it = fifth.begin(); it != fifth.end(); it++)
    std::cout << *it << ' ';

  std::cout << '\n';

  return 0;
}

/*
Output:

The contents of fifth are: 16 2 77 29 
*/

