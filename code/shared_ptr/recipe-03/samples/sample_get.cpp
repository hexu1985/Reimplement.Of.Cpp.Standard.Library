// shared_ptr::get example
#include <iostream>
#include "shared_ptr.hpp"

using mini_stl::shared_ptr;

int main () {
    int* p = new int (10);
    shared_ptr<int> a (p);

    if (a.get()==p)
        std::cout << "a and p point to the same location\n";

    // three ways of accessing the same address:
    std::cout << *a.get() << "\n";
    std::cout << *a << "\n";
    std::cout << *p << "\n";

    return 0;
}

/*
Output:
a and p point to the same location
10
10
10
*/
