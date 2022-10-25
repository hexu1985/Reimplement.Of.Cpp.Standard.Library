// unordered_map hash table stats
#include <iostream>
#include <unordered_map>

int main ()
{
  std::unordered_map<int,int> mymap;

  std::cout << "size = " << mymap.size() << std::endl;
  std::cout << "bucket_count = " << mymap.bucket_count() << std::endl;
  std::cout << "load_factor = " << mymap.load_factor() << std::endl;
  std::cout << "max_load_factor = " << mymap.max_load_factor() << std::endl;

  return 0;
}
