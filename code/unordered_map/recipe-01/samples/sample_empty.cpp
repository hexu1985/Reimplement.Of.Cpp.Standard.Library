// unordered_map::empty
#include <iostream>
#include "unordered_map.hpp"

int main ()
{
  Hx::unordered_map<int,int> first;
  Hx::unordered_map<int,int> second = {{1,10},{2,20},{3,30}};
  std::cout << "first " << (first.empty() ? "is empty" : "is not empty" ) << std::endl;
  std::cout << "second " << (second.empty() ? "is empty" : "is not empty" ) << std::endl;
  return 0;
}
