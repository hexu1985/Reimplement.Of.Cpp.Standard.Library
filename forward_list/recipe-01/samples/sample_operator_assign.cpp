// assignment operator with forward_list
#include <iostream>
#include "forward_list.hpp"

template<class Container>
Container by_two (const Container& x) 
{
  Container temp(x); 
  typedef typename Container::iterator iterator;
  for (iterator it = temp.begin(); it != temp.end(); ++it) 
    *it *= 2; 
  return temp;
}

int main ()
{
  mini_stl::forward_list<int> first (4);      // 4 ints
  mini_stl::forward_list<int> second (3,5);   // 3 ints with value 5

  first = second;                        // copy assignment
  second = by_two(first);                // move assignment

  typedef typename mini_stl::forward_list<int>::iterator iterator;
  std::cout << "first: ";
  for (iterator it = first.begin(); it != first.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "second: ";
  for (iterator it = second.begin(); it != second.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

/*
In the first assignment, second is an lvalue: the copy assignment function is called.
In the second assignment, the value returned by by_two(first) is an rvalue: the move assignment function is called.
Output:

first: 5 5 5
second: 10 10 10
*/

