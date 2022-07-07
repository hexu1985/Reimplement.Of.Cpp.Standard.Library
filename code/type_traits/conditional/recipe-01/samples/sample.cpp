#include <iostream>
#include <typeinfo>
#include "conditional.hpp"
 
int main() 
{
    typedef Hx::conditional<true, int, double>::type Type1;
    typedef Hx::conditional<false, int, double>::type Type2;
    typedef Hx::conditional<sizeof(int) >= sizeof(double), int, double>::type Type3;
 
    std::cout << typeid(Type1).name() << '\n';
    std::cout << typeid(Type2).name() << '\n';
    std::cout << typeid(Type3).name() << '\n';
}
