#include <iostream>
#include "unique_ptr.hpp"
 
int main()
{
    mini_stl::unique_ptr<int> p1(new int(42));
    mini_stl::unique_ptr<int> p2(new int(42));
 
    std::cout << std::boolalpha
        << "(p1 == p1)       : " << (p1 == p1) << '\n'
        // << "(p1 <=> p1) == 0 : " << ((p1 <=> p1) == 0) << '\n' // Since C++20
 
    // p1 and p2 point to different memory locations, so p1 != p2
        << "(p1 == p2)       : " << (p1 == p2) << '\n'
        << "(p1 < p2)        : " << (p1 < p2) << '\n';
        //<< "(p1 <=> p2) < 0  : " << ((p1 <=> p2) < 0) << '\n'   // Since C++20
        //<< "(p1 <=> p2) == 0 : " << ((p1 <=> p2) == 0) << '\n'; // Since C++20
}

/*
Possible output:

(p1 == p1)       : true
(p1 <=> p1) == 0 : true
(p1 == p2)       : false
(p1 < p2)        : true
(p1 <=> p2) < 0  : true
(p1 <=> p2) == 0 : false
*/
