#include <cstddef>
#include <iostream>
#if 0
#include <span>
#endif
#include "vector.hpp"
 
void pointer_func(const int* p, std::size_t size)
{
    std::cout << "data = ";
    for (std::size_t i = 0; i < size; ++i)
        std::cout << p[i] << ' ';
    std::cout << '\n';
}
 
#if 0
void span_func(std::span<const int> data) // since C++20
{
    std::cout << "data = ";
    for (const int e : data)
        std::cout << e << ' ';
    std::cout << '\n';
}
#endif
 
int main()
{
    mini_stl::vector<int> container { 1, 2, 3, 4 };
 
    // Prefer container.data() over &container[0]
    pointer_func(container.data(), container.size());
 
#if 0
    // std::span (C++20) is a safer alternative to separated pointer/size.
    span_func({container.data(), container.size()});
#endif
}
