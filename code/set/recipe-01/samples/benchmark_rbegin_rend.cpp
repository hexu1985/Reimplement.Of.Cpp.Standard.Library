// set::rbegin/rend
#include <iostream>
#include <set>

int main ()
{
  int myints[] = {21,64,17,78,49};
  std::set<int> myset (myints,myints+5);

  std::set<int>::reverse_iterator rit;

  std::cout << "myset contains:";
  for (rit=myset.rbegin(); rit != myset.rend(); ++rit)
    std::cout << ' ' << *rit;

  std::cout << '\n';

  return 0;
}

/*
Output:

myset contains: 78 64 49 21 17
*/
