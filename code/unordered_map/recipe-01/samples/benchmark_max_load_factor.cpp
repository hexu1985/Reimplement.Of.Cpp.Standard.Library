// unordered_map::max_load_factor
#include <iostream>
#include <string>
#include <unordered_map>

int main ()
{
  std::unordered_map<std::string,std::string> mymap = {
    {"Au","gold"},
    {"Ag","Silver"},
    {"Cu","Copper"},
    {"Pt","Platinum"}
  };

  std::cout << "current max_load_factor: " << mymap.max_load_factor() << std::endl;
  std::cout << "current size: " << mymap.size() << std::endl;
  std::cout << "current bucket_count: " << mymap.bucket_count() << std::endl;
  std::cout << "current load_factor: " << mymap.load_factor() << std::endl;

  float z = mymap.max_load_factor();
  mymap.max_load_factor ( z / 2.0 );
  std::cout << "[max_load_factor halved]" << std::endl;

  std::cout << "new max_load_factor: " << mymap.max_load_factor() << std::endl;
  std::cout << "new size: " << mymap.size() << std::endl;
  std::cout << "new bucket_count: " << mymap.bucket_count() << std::endl;
  std::cout << "new load_factor: " << mymap.load_factor() << std::endl;

  return 0;
}
