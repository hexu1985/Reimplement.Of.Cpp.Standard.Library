// shared_ptr::owner_before
#include <iostream>
#include "shared_ptr.hpp"

using mini_stl::shared_ptr;

int main () {
    int * p = new int (10);

    shared_ptr<int> a (new int (20));
    shared_ptr<int> b (a,p);  // alias constructor

    std::cout << "comparing a and b...\n" << std::boolalpha;
    std::cout << "value-based: " << ( !(a<b) && !(b<a) ) << '\n';
    std::cout << "owner-based: " << ( !a.owner_before(b) && !b.owner_before(a) ) << '\n';

    delete p;
    return 0;
}



/*
Output:

comparing a and b...
value-based: false
owner-based: true
*/
