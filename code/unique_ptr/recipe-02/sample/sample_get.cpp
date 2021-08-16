#include <iostream>
#include <string>
#include "unique_ptr.hpp"
 
int main()
{
    mini_stl::unique_ptr<std::string> s_p(new std::string("Hello, world!"));
    std::string *s = s_p.get();
    std::cout << *s << '\n';
}

/*
Output:

Hello, world!
*/
