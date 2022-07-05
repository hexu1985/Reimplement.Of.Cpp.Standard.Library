// duration::min/max
#include <iostream>
#include "chrono.hpp"

int main ()
{
    std::cout << "system_clock durations can represent:\n";
    std::cout << "min: " << Hx::chrono::system_clock::duration::min().count() << "\n";
    std::cout << "max: " << Hx::chrono::system_clock::duration::max().count() << "\n";
    return 0;
}
