#include <iostream>
#include "unique_ptr.hpp"
 
int main()
{
    Hx::unique_ptr<int> ptr(new int(42));
 
    if (ptr) std::cout << "before reset, ptr is: " << *ptr << '\n';
    ptr.reset();
    if (ptr) std::cout << "after reset, ptr is: " << *ptr << '\n';
}

/*
Output:

before reset, ptr is: 42
*/
