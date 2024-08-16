// forward_list::pop_front
#include <iostream>
#include <forward_list>

int main ()
{
  std::forward_list<int> mylist;
  mylist.push_front (100);
  mylist.push_front (200);
  mylist.push_front (300);

  std::cout << "Popping out the elements in mylist:";
  while (!mylist.empty())
  {
    std::cout << ' ' << mylist.front();
    mylist.pop_front();
  }
  std::cout << '\n';

  return 0;
}

/*
Output:
Popping out the elements in mylist: 300 200 100
*/

