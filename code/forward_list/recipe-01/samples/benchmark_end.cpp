// forward_list::end example
#include <iostream>
#include <forward_list>

int main ()
{
  std::forward_list<int> mylist = { 10, 20, 30, 40 };

  std::cout << "mylist contains:";
  for ( auto it = mylist.begin(); it != mylist.end(); ++it )
    std::cout << ' ' << *it;

  std::cout << '\n';

  return 0;
}


/*
Output:

mylist contains: 10 20 30 40
*/


