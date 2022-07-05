// shared_ptr::reset example
#include <iostream>
#include "shared_ptr.hpp"

using Hx::shared_ptr;

int main () {
    shared_ptr<int> sp;  // empty

    sp.reset (new int);       // takes ownership of pointer
    *sp=10;
    std::cout << *sp << '\n';

    sp.reset (new int);       // deletes managed object, acquires new pointer
    *sp=20;
    std::cout << *sp << '\n';

    sp.reset();               // deletes managed object

    return 0;
}

/*
Output:
10
20
*/

