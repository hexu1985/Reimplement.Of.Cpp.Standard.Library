// forward_list::sort
#include <iostream>
#include <forward_list>
#include <functional>

int main ()
{
  std::forward_list<int> mylist = {22, 13, 5, 40, 90, 62, 31};

  mylist.sort();

  std::cout << "default sort (operator<):";
  for (int& x: mylist) std::cout << ' ' << x;
  std::cout << '\n';

  mylist.sort(std::greater<int>());

  std::cout << "sort with std::greater():";
  for (int& x: mylist) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}


/*
Output:

default sort (operator<): 5 13 22 31 40 62 90
sort with std::greater(): 90 62 40 31 22 13 5
*/
