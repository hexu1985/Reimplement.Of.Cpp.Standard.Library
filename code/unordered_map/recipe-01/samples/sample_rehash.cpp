// unordered_map::rehash
#include <iostream>
#include <string>
#include "unordered_map.hpp"

int main ()
{
  Hx::unordered_map<std::string,std::string> mymap;

  mymap.rehash(20);

  mymap["house"] = "maison";
  mymap["apple"] = "pomme";
  mymap["tree"] = "arbre";
  mymap["book"] = "livre";
  mymap["door"] = "porte";
  mymap["grapefruit"] = "pamplemousse";

  std::cout << "current bucket_count: " << mymap.bucket_count() << std::endl;

  return 0;
}
