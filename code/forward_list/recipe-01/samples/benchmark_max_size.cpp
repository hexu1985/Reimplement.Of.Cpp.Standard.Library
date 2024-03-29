// forward_list::max_size
#include <iostream>
#include <sstream>
#include <forward_list>

int main ()
{
  int myint;
  std::string mystring;
  std::forward_list<int> mylist;

  std::cout << "Enter size: ";
  std::getline ( std::cin, mystring );
  std::stringstream ( mystring ) >> myint;

  if ( myint <= mylist.max_size() ) mylist.resize(myint);
  else std::cout << "That size exceeds the maximum.\n";

  return 0;
}

