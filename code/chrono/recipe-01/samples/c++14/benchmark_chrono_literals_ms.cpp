#include <iostream>
#include <chrono>
 
int main()
{
    using namespace std::chrono_literals;
    auto d1 = 250ms;
    std::chrono::milliseconds d2 = 1s;
    std::cout << "250ms = " << d1.count() << " milliseconds\n"
              << "1s = " << d2.count() << " milliseconds\n";
}
