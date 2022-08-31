// forward_list::splice_after
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  Hx::forward_list<int> first = { 1, 2, 3 };
  Hx::forward_list<int> second = { 10, 20, 30 };

  auto it = first.begin();  // points to the 1

  first.splice_after ( first.before_begin(), second );
                          // first: 10 20 30 1 2 3
                          // second: (empty)
                          // "it" still points to the 1 (now first's 4th element)

  second.splice_after ( second.before_begin(), first, first.begin(), it);
                          // first: 10 1 2 3
                          // second: 20 30

  first.splice_after ( first.before_begin(), second, second.begin() );
                          // first: 30 10 1 2 3
                          // second: 20
                          // * notice that what is moved is AFTER the iterator

  std::cout << "first contains:";
  for (int& x: first) std::cout << " " << x;
  std::cout << std::endl;

  std::cout << "second contains:";
  for (int& x: second) std::cout << " " << x;
  std::cout << std::endl;

  return 0;
}

/*
Output:

first contains: 30 10 1 2 3
second contains: 20
*/
