// clearing forward_list
#include <iostream>
#include <forward_list>

int main ()
{
  std::forward_list<int> mylist = { 10, 20, 30 };

  std::cout << "mylist contains:";
  for (int& x: mylist) std::cout << ' ' << x;
  std::cout << '\n';

  mylist.clear();
  mylist.insert_after( mylist.before_begin(), {100, 200} );

  std::cout << "mylist contains:";
  for (int& x: mylist) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}


/*
Output:

mylist contains: 10 20 30
mylist contains: 100 200
*/
