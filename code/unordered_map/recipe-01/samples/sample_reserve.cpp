// unordered_map::reserve
#include <iostream>
#include <string>
#include "unordered_map.hpp"

int main ()
{
  Hx::unordered_map<std::string,std::string> mymap;

  mymap.reserve(6);

  mymap["house"] = "maison";
  mymap["apple"] = "pomme";
  mymap["tree"] = "arbre";
  mymap["book"] = "livre";
  mymap["door"] = "porte";
  mymap["grapefruit"] = "pamplemousse";

  for (auto& x: mymap) {
    std::cout << x.first << ": " << x.second << std::endl;
  }

  return 0;
}
