// resizing forward_list
#include <iostream>
#include <forward_list>

int main ()
{
  std::forward_list<int> mylist = {10, 20, 30, 40, 50};
                                // 10 20 30 40 50
  mylist.resize(3);             // 10 20 30
  mylist.resize(5,100);         // 10 20 30 100 100

  std::cout << "mylist contains:";
  for (int& x: mylist) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}


/*
mylist contains: 10 20 30 100 100
*/

