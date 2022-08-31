// forward_list::unique
#include <iostream>
#include <cmath>
#include "forward_list.hpp"

// a binary predicate implemented as a function:
bool same_integral_part (double first, double second)
{ return ( int(first)==int(second) ); }

// a binary predicate implemented as a class:
class is_near_class
{
public:
  bool operator() (double first, double second)
  { return (fabs(first-second)<5.0); }
} is_near_object;

int main ()
{

  Hx::forward_list<double> mylist = { 15.2, 73.0, 3.14, 15.85, 69.5,
                                       73.0, 3.99, 15.2, 69.2,  18.5 };

  mylist.sort();                       //   3.14,  3.99, 15.2, 15.2, 15.85
                                       //  18.5,  69.2,  69.5, 73.0, 73.0

  mylist.unique();                     //   3.14,  3.99, 15.2, 15.85
                                       //  18.5,  69.2,  69.5, 73.0

  mylist.unique (same_integral_part);  //  3.14, 15.2, 18.5,  69.2, 73.0

  mylist.unique (is_near_object);      //  3.14, 15.2, 69.2

  std::cout << "mylist contains:";
  for (double& x: mylist) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}


/*
Output:

mylist contains: 3.14 15.2 69.2
*/

