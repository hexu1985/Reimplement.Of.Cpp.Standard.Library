// forward_list::insert_after
#include <iostream>
#include <array>
#include "forward_list.hpp"

int main ()
{
  std::array<int,3> myarray = { 11, 22, 33 };
  mini_stl::forward_list<int> mylist;
  mini_stl::forward_list<int>::iterator it;

  it = mylist.insert_after ( mylist.before_begin(), 10 );          // 10
                                                                   //  ^  <- it
  it = mylist.insert_after ( it, 2, 20 );                          // 10 20 20
                                                                   //        ^
  it = mylist.insert_after ( it, myarray.begin(), myarray.end() ); // 10 20 20 11 22 33
                                                                   //                 ^
  it = mylist.begin();                                             //  ^
  it = mylist.insert_after ( it, {1,2,3} );                        // 10 1 2 3 20 20 11 22 33
                                                                   //        ^

  std::cout << "mylist contains:";
  for (int& x: mylist) std::cout << ' ' << x;
  std::cout << '\n';
  return 0;
}

/*
Output:

mylist contains: 10 1 2 3 20 20 11 22 33
*/

