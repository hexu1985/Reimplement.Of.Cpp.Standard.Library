// const_pointer_cast example
#include <iostream>
#include "shared_ptr.hpp"

using mini_stl::shared_ptr;
using mini_stl::make_shared;
using mini_stl::const_pointer_cast;

int main () {
    shared_ptr<int> foo;
    shared_ptr<const int> bar;

    foo = make_shared<int>(10);

    bar = const_pointer_cast<const int>(foo);

    std::cout << "*bar: " << *bar << '\n';
    *foo = 20;
    std::cout << "*bar: " << *bar << '\n';

    return 0;
}


/*
Output:

*bar: 10
*bar: 20
*/
