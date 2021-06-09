// remove from forward_list
#include <iostream>
#include <forward_list>

int main ()
{
  int myints[]= {17,89,7,14};
  std::forward_list<int> mylist (myints,myints+4);

  mylist.remove(89);

  std::cout << "mylist contains:";
  for (std::forward_list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}


/*
Output:

mylist contains: 17 7 14
*/

