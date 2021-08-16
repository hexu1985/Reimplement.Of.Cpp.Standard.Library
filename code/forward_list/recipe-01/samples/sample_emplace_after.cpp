// forward_list::emplace_after
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  mini_stl::forward_list< std::pair<int,char> > mylist;
  auto it = mylist.before_begin();

  it = mylist.emplace_after ( it, 100, 'x' );
  it = mylist.emplace_after ( it, 200, 'y' );
  it = mylist.emplace_after ( it, 300, 'z' );

  std::cout << "mylist contains:";
  for (auto& x: mylist)
    std::cout << " (" << x.first << "," << x.second << ")";

  std::cout << '\n';
  return 0;
}

/*
Output:

mylist contains: (100,x) (200,y) (300,z)
*/
