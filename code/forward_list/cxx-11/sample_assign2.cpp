// forward_list::assign
#include <iostream>
#include <forward_list>

int main ()
{
  std::forward_list<int> first;
  std::forward_list<int> second;

  first.assign (4,15);                           // 15 15 15 15

  second.assign (first.begin(),first.end());     // 15 15 15 15

  first.assign ( {77, 2, 16} );                  // 77 2 16

  std::cout << "first contains: ";
  for (int& x : first) std::cout << ' ' << x;
  std::cout << '\n';

  std::cout << "second contains: ";
  for (int& x : second) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}

/*
Output:

first contains: 77 2 16
second contains: 15 15 15 15
*/

