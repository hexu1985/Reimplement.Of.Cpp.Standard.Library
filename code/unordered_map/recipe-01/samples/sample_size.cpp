// unordered_map::size
#include <iostream>
#include <string>
#include "unordered_map.hpp"

int main ()
{
  Hx::unordered_map<std::string,double> mymap = {
       {"milk",2.30},
       {"potatoes",1.90},
       {"eggs",0.40}
  };

  std::cout << "mymap.size() is " << mymap.size() << std::endl;

  return 0;
}
