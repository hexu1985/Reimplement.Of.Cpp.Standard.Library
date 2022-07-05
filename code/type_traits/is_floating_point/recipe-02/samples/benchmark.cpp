#include <iostream>
#include <type_traits>
 
class A {};
 
int main() 
{
    std::cout << std::boolalpha;
    std::cout << "      A: " << std::is_floating_point<A>::value << '\n';
    std::cout << "  float: " << std::is_floating_point<float>::value << '\n';
    std::cout << " float&: " << std::is_floating_point<float&>::value << '\n';
    std::cout << " double: " << std::is_floating_point<double>::value << '\n';
    std::cout << "double&: " << std::is_floating_point<double&>::value << '\n';
    std::cout << "    int: " << std::is_floating_point<int>::value << '\n';
}
