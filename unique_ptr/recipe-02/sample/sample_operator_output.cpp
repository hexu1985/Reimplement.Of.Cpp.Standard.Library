#include <iostream>
#include "unique_ptr.hpp"
 
class Foo {};
 
int main()
{
    auto p = mini_stl::make_unique<Foo>();
    std::cout << p << '\n';
    std::cout << p.get() << '\n';
}

/*
Possible output:

0x6d9028
0x6d9028
*/
