// unordered_map comparisons
#include <iostream>
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string,std::string> stringmap;

int main ()
{
  stringmap a = { {"AAPL","Apple"}, {"MSFT","Microsoft"}, {"GOOG","Google"} };
  stringmap b = { {"MSFT","Microsoft"}, {"GOOG","Google"}, {"AAPL","Apple"} };
  stringmap c = { {"MSFT","Microsoft Corp."}, {"GOOG","Google Inc."}, {"AAPL","Apple Inc."} };

  if (a==b) std::cout << "a and b are equal\n";
  if (b!=c) std::cout << "b and c are not equal\n";

  return 0;
}
