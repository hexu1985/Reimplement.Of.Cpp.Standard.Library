// unordered_map::operator[]
#include <iostream>
#include <string>
#include "unordered_map.hpp"

int main ()
{
  Hx::unordered_map<std::string,std::string> mymap;

  mymap["Bakery"]="Barbara";  // new element inserted
  mymap["Seafood"]="Lisa";    // new element inserted
  mymap["Produce"]="John";    // new element inserted

  std::string name = mymap["Bakery"];   // existing element accessed (read)
  mymap["Seafood"] = name;              // existing element accessed (written)

  mymap["Bakery"] = mymap["Produce"];   // existing elements accessed (read/written)

  name = mymap["Deli"];      // non-existing element: new element "Deli" inserted!

  mymap["Produce"] = mymap["Gifts"];    // new element "Gifts" inserted, "Produce" written

  for (auto& x: mymap) {
    std::cout << x.first << ": " << x.second << std::endl;
  }

  return 0;
}
