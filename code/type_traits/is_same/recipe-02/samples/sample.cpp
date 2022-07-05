#include <iostream>
#include <type_traits>
#include <cstdint>
#include "is_same.hpp"
 
void print_separator()
{
    std::cout << "-----\n";
}
 
int main()
{
    std::cout << std::boolalpha;
 
    // some implementation-defined facts
 
    // usually true if 'int' is 32 bit
    std::cout << Hx::is_same<int, std::int32_t>::value << ' '; // ~ true
    // possibly true if ILP64 data model is used
    std::cout << Hx::is_same<int, std::int64_t>::value << ' '; // ~ false
 
    // same tests as above, except using C++17's `Hx::is_same_v<T, U>` format
    std::cout << Hx::is_same_v<int, std::int32_t> << ' ';  // ~ true
    std::cout << Hx::is_same_v<int, std::int64_t> << '\n'; // ~ false
 
    print_separator();
 
    // compare the types of a couple variables
    long double num1 = 1.0;
    long double num2 = 2.0;
    std::cout << Hx::is_same_v<decltype(num1), decltype(num2)> << '\n'; // true
 
    print_separator();
 
    // 'float' is never an integral type
    std::cout << Hx::is_same<float, std::int32_t>::value << '\n'; // false
 
    print_separator();
 
    // 'int' is implicitly 'signed'
    std::cout << Hx::is_same<int, int>::value << ' ';          // true
    std::cout << Hx::is_same<int, unsigned int>::value << ' '; // false
    std::cout << Hx::is_same<int, signed int>::value << '\n';  // true
 
    print_separator();
 
    // unlike other types, 'char' is neither 'unsigned' nor 'signed'
    std::cout << Hx::is_same<char, char>::value << ' ';          // true
    std::cout << Hx::is_same<char, unsigned char>::value << ' '; // false
    std::cout << Hx::is_same<char, signed char>::value << '\n';  // false
 
    // const-qualified type T is not same as non-const T
    static_assert( not Hx::is_same<const int, int>() );
}
