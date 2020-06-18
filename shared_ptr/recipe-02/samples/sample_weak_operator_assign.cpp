// weak_ptr::operator= example
#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using mini_stl::shared_ptr;
using mini_stl::weak_ptr;
using mini_stl::make_shared;

int main () {
    shared_ptr<int> sp1,sp2;
    weak_ptr<int> wp;
                                         // sharing group:
                                         // --------------
    sp1 = make_shared<int> (10);         // sp1
    wp = sp1;                            // sp1, wp

    sp2 = wp.lock();                     // sp1, wp, sp2
    sp1.reset();                         //      wp, sp2

    sp1 = wp.lock();                     // sp1, wp, sp2

    std::cout << "*sp1: " << *sp1 << '\n';
    std::cout << "*sp2: " << *sp2 << '\n';

    return 0;
}

/*
Output:
*sp1: 10
*sp2: 10
*/
