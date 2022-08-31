// forward_list::remove_if
#include <iostream>
#include "forward_list.hpp"

// a predicate implemented as a function:
bool single_digit (const int& value) { return (value<10); }

// a predicate implemented as a class:
class is_odd_class
{
public:
  bool operator() (const int& value) {return (value%2)==1; }
} is_odd_object;

int main ()
{
  Hx::forward_list<int> mylist = {7, 80, 7, 15, 85, 52, 6};

  mylist.remove_if (single_digit);      // 80 15 85 52

  mylist.remove_if (is_odd_object);     // 80 52

  std::cout << "mylist contains:";
  for (int& x: mylist) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}


/*
Output:

mylist contains: 80 52
*/
