#include <iostream>
#include "chrono.hpp"
#include "chrono_literals.hpp"
 
int main()
{
    using namespace mini_stl::chrono_literals;
    auto d1 = 250ns;
    mini_stl::chrono::nanoseconds d2 = 1us;
    std::cout << "250ns = " << d1.count() << " nanoseconds\n"
              << "1us = " << d2.count() << " nanoseconds\n";
}
