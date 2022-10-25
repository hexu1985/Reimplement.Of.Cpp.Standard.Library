// unordered_map limits
#include <iostream>
#include "unordered_map.hpp"

int main ()
{
  Hx::unordered_map<int,int> mymap;

  std::cout << "max_size = " << mymap.max_size() << std::endl;
  std::cout << "max_bucket_count = " << mymap.max_bucket_count() << std::endl;
  std::cout << "max_load_factor = " << mymap.max_load_factor() << std::endl;

  return 0;
}
