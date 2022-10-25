// clearing unordered_map
#include <iostream>
#include <string>
#include <unordered_map>

int main ()
{
  std::unordered_map<std::string,std::string> mymap =
         { {"house","maison"}, {"car","voiture"}, {"grapefruit","pamplemousse"} };

  std::cout << "mymap contains:";
  for (auto& x: mymap) std::cout << " " << x.first << "=" << x.second;
  std::cout << std::endl;

  mymap.clear();
  mymap["hello"]="bonjour";
  mymap["sun"]="soleil";

  std::cout << "mymap contains:";
  for (auto& x: mymap) std::cout << " " << x.first << "=" << x.second;
  std::cout << std::endl;

  return 0;
}
