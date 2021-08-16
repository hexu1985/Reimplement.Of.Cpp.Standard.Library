// weak_ptr::reset example
#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using mini_stl::shared_ptr;
using mini_stl::weak_ptr;

int main () 
{
    shared_ptr<int> sp (new int(10));

    weak_ptr<int> wp(sp);

    std::cout << "1. wp " << (wp.expired()?"is":"is not") << " expired\n";

    wp.reset();

    std::cout << "2. wp " << (wp.expired()?"is":"is not") << " expired\n";

    return 0;
}


/*
Output:

1. wp is not expired
2. wp is expired
*/
