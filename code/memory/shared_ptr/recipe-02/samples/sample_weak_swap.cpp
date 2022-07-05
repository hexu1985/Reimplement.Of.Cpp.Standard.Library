// weak_ptr::swap example
#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using Hx::shared_ptr;
using Hx::weak_ptr;

int main () 
{
    shared_ptr<int> sp1 (new int(10));
    shared_ptr<int> sp2 (new int(20));

    weak_ptr<int> wp1(sp1);
    weak_ptr<int> wp2(sp2);

    wp1.swap(wp2);

    std::cout << "sp1 -> " << *sp1 << '\n';
    std::cout << "sp2 -> " << *sp2 << '\n';
    std::cout << "wp1 -> " << *wp1.lock() << '\n';
    std::cout << "wp2 -> " << *wp2.lock() << '\n';

    return 0;
}


/*
Output:

sp1 -> 10
sp2 -> 20
wp1 -> 20
wp2 -> 10
*/
