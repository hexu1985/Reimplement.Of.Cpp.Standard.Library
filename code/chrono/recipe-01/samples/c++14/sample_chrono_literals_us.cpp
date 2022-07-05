#include <iostream>
#include "chrono.hpp"
#include "chrono_literals.hpp"
 
int main()
{
    using namespace Hx::chrono_literals;
    auto d1 = 250us;
    Hx::chrono::microseconds d2 = 1ms;
    std::cout << "250us = " << d1.count() << " microseconds\n"
              << "1ms = " << d2.count() << " microseconds\n";
}
