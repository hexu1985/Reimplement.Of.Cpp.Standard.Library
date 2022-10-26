// unordered_map::key_eq
#include <iostream>
#include <string>
#include "unordered_map.hpp"

int main ()
{
  Hx::unordered_map<std::string,std::string> mymap;

  bool case_insensitive = mymap.key_eq()("test","TEST");

  std::cout << "mymap.key_eq() is ";
  std::cout << ( case_insensitive ? "case insensitive" : "case sensitive" );
  std::cout << std::endl;

  return 0;
}
