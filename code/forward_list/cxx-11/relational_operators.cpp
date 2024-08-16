// forward_list comparisons
#include <iostream>
#include <forward_list>

int main ()
{
  std::forward_list<int> a = {10, 20, 30};
  std::forward_list<int> b = {10, 20, 30};
  std::forward_list<int> c = {30, 20, 10};

  if (a==b) std::cout << "a and b are equal\n";
  if (b!=c) std::cout << "b and c are not equal\n";
  if (b<c) std::cout << "b is less than c\n";
  if (c>b) std::cout << "c is greater than b\n";
  if (a<=b) std::cout << "a is less than or equal to b\n";
  if (a>=b) std::cout << "a is greater than or equal to b\n";

  return 0;
}


/*
Output:

a and b are equal
b and c are not equal
b is less than c
c is greater than b
a is less than or equal to b
a is greater than or equal to b
*/
