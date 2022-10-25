// constructing unordered_maps
#include <iostream>
#include <string>
#include "unordered_map.hpp"

typedef Hx::unordered_map<std::string,std::string> stringmap;

stringmap merge (stringmap a,stringmap b) {
  stringmap temp(a); temp.insert(b.begin(),b.end()); return temp;
}

int main ()
{
  stringmap first;                              // empty
  stringmap second ( {{"apple","red"},{"lemon","yellow"}} );       // init list
  stringmap third ( {{"orange","orange"},{"strawberry","red"}} );  // init list
  stringmap fourth (second);                    // copy
  stringmap fifth (merge(third,fourth));        // move
  stringmap sixth (fifth.begin(),fifth.end());  // range

  std::cout << "sixth contains:";
  for (auto& x: sixth) std::cout << " " << x.first << ":" << x.second;
  std::cout << std::endl;

  return 0;
}
