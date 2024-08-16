// forward_list::begin/end
#include <iostream>
#include <forward_list>

int main ()
{
  int myints[] = {75,23,65,42,13};
  std::forward_list<int> mylist (myints,myints+5);

  std::cout << "mylist contains:";
  for (std::forward_list<int>::iterator it=mylist.begin(); it != mylist.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';

  return 0;
} 


/*
Output:

mylist contains: 75 23 65 42 13
*/


