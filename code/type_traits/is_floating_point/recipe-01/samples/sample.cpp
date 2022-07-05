#include <iostream>
#include <type_traits>
#include "is_floating_point.hpp"
 
class A {};
 
int main() 
{
    std::cout << std::boolalpha;
    std::cout << "      A: " << Hx::is_floating_point<A>::value << '\n';
    std::cout << "  float: " << Hx::is_floating_point<float>::value << '\n';
    std::cout << " float&: " << Hx::is_floating_point<float&>::value << '\n';
    std::cout << " double: " << Hx::is_floating_point<double>::value << '\n';
    std::cout << "double&: " << Hx::is_floating_point<double&>::value << '\n';
    std::cout << "    int: " << Hx::is_floating_point<int>::value << '\n';
}
