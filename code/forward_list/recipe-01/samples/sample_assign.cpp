// forward_list::assign
#include <iostream>
#include "forward_list.hpp"

int main ()
{
  mini_stl::forward_list<int> first;
  mini_stl::forward_list<int> second;

  first.assign (7,100);                      // 7 ints with value 100

  second.assign (first.begin(),first.end()); // a copy of first

  int myints[]={1776,7,4};
  first.assign (myints,myints+3);            // assigning from array

  std::cout << "Size of first: " << int (std::distance(first.begin(), first.end())) << '\n';
  std::cout << "Size of second: " << int (std::distance(second.begin(), second.end())) << '\n';
  return 0;
}

/*
Output:

Size of first: 3
Size of second: 7
*/

