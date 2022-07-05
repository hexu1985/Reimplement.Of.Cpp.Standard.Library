// set::get_allocator
#include <iostream>
#include "set.hpp"

int main ()
{
  Hx::set<int> myset;
  int * p;
  unsigned int i;

  // allocate an array of 5 elements using myset's allocator:
  p=myset.get_allocator().allocate(5);

  // assign some values to array
  for (i=0; i<5; i++) p[i]=(i+1)*10;

  std::cout << "The allocated array contains:";
  for (i=0; i<5; i++) std::cout << ' ' << p[i];
  std::cout << '\n';

  myset.get_allocator().deallocate(p,5);

  return 0;
}
