// forward_list::merge
#include <iostream>
#include "forward_list.hpp"

// compare only integral part:
bool mycomparison (double first, double second)
{ return ( int(first)<int(second) ); }

int main ()
{
  Hx::forward_list<double> first, second;

  first.push_front (3.1);
  first.push_front (2.2);
  first.push_front (2.9);

  second.push_front (3.7);
  second.push_front (7.1);
  second.push_front (1.4);

  first.sort();
  second.sort();

  first.merge(second);

  // (second is now empty)

  second.push_front (2.1);

  first.merge(second,mycomparison);

  std::cout << "first contains:";
  for (Hx::forward_list<double>::iterator it=first.begin(); it!=first.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

/*
Output:
first contains: 1.4 2.2 2.9 2.1 3.1 3.7 7.1
*/

