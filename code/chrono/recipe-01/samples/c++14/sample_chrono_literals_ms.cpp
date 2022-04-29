#include <iostream>
#include "chrono.hpp"
#include "chrono_literals.hpp"
 
int main()
{
    using namespace mini_stl::chrono_literals;
    auto d1 = 250ms;
    mini_stl::chrono::milliseconds d2 = 1s;
    std::cout << "250ms = " << d1.count() << " milliseconds\n"
              << "1s = " << d2.count() << " milliseconds\n";
}
