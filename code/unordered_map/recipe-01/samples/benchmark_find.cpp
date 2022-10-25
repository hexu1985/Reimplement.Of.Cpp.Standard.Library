// unordered_map::find
#include <iostream>
#include <string>
#include <unordered_map>

int main ()
{
  std::unordered_map<std::string,double> mymap = {
     {"mom",5.4},
     {"dad",6.1},
     {"bro",5.9} };

  std::string input;
  std::cout << "who? ";
  getline (std::cin,input);

  std::unordered_map<std::string,double>::const_iterator got = mymap.find (input);

  if ( got == mymap.end() )
    std::cout << "not found";
  else
    std::cout << got->first << " is " << got->second;

  std::cout << std::endl;

  return 0;
}
