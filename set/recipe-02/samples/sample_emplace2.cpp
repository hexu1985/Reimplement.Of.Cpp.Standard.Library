// set::emplace
#include <iostream>
#include "set.hpp"
#include <string>

int main ()
{
  mini_stl::set<std::string> myset;

  myset.emplace("foo");
  myset.emplace("bar");
  auto ret = myset.emplace("foo");

  if (!ret.second) std::cout << "foo already exists in myset\n";

  return 0;
}

/*
Output:
foo already exists in myset
*/
