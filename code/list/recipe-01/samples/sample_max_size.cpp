#include <iostream>
#include "list.hpp"
 
int main()
{
    Hx::list<char> s;
    std::cout << "Maximum size of a 'list' is " << s.max_size() << "\n";
}
