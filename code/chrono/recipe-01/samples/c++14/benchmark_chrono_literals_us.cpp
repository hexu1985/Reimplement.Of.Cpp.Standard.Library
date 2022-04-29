#include <iostream>
#include <chrono>
 
int main()
{
    using namespace std::chrono_literals;
    auto d1 = 250us;
    std::chrono::microseconds d2 = 1ms;
    std::cout << "250us = " << d1.count() << " microseconds\n"
              << "1ms = " << d2.count() << " microseconds\n";
}
