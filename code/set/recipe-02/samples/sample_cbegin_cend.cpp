#include <algorithm>
#include <iostream>
#include "set.hpp"
 
int main() {
  Hx::set<int> set = { 3, 1, 4, 1, 5, 9, 2, 6, 5 };
  std::for_each(set.cbegin(), set.cend(), [](int x) {
    std::cout << x << ' ';
  });
  std::cout << '\n';
}

/*
Output:

1 2 3 4 5 6 9
*/
