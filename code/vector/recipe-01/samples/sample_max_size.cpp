#include "vector.hpp"
#include <iostream>
 
int main()
{
    mini_stl::vector<char> s;
    std::cout << "Maximum size of a 'vector' is " << s.max_size() << "\n";
}
