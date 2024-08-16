// forward_list::merge
#include <iostream>
#include "forward_list.hpp"
#include <functional>

int main ()
{
  Hx::forward_list<double> first = {4.2, 2.9, 3.1};
  Hx::forward_list<double> second = {1.4, 7.7, 3.1};
  Hx::forward_list<double> third = {6.2, 3.7, 7.1};

  first.sort();
  second.sort();
  first.merge(second);

  std::cout << "first contains:";
  for (double& x: first) std::cout << " " << x;
  std::cout << std::endl;

  first.sort (std::greater<double>());
  third.sort (std::greater<double>());
  first.merge (third, std::greater<double>());

  std::cout << "first contains:";
  for (double& x: first) std::cout << " " << x;
  std::cout << std::endl;

  return 0;
}


/*
Output:

first contains: 1.4 2.9 3.1 3.1 4.2 7.7
first contains: 7.7 7.1 6.2 4.2 3.7 3.1 3.1 2.9 1.4
*/
