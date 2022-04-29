#include <iostream>
#include <chrono>
 
int main()
{
    using namespace std::chrono_literals;
    auto d1 = 250ns;
    std::chrono::nanoseconds d2 = 1us;
    std::cout << "250ns = " << d1.count() << " nanoseconds\n"
              << "1us = " << d2.count() << " nanoseconds\n";
}
