// unordered_map::bucket_size
#include <iostream>
#include <string>
#include "unordered_map.hpp"

int main ()
{
  Hx::unordered_map<std::string,std::string> mymap = {
    {"us","United States"},
    {"uk","United Kingdom"},
    {"fr","France"},
    {"de","Germany"}
  };

  unsigned nbuckets = mymap.bucket_count();

  std::cout << "mymap has " << nbuckets << " buckets:\n";

  for (unsigned i=0; i<nbuckets; ++i) {
    std::cout << "bucket #" << i << " has " << mymap.bucket_size(i) << " elements.\n";
  }

  return 0;
}
