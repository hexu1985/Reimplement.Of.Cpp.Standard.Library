// unordered_map::bucket
#include <iostream>
#include <string>
#include <unordered_map>

int main ()
{
  std::unordered_map<std::string,std::string> mymap = {
    {"us","United States"},
    {"uk","United Kingdom"},
    {"fr","France"},
    {"de","Germany"}
  };

  for (auto& x: mymap) {
    std::cout << "Element [" << x.first << ":" << x.second << "]";
    std::cout << " is in bucket #" << mymap.bucket (x.first) << std::endl;
  }

  return 0;
}
