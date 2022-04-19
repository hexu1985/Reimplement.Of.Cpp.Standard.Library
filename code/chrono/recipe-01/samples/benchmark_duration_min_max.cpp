// duration::min/max
#include <iostream>
#include <chrono>

int main ()
{
    std::cout << "system_clock durations can represent:\n";
    std::cout << "min: " << std::chrono::system_clock::duration::min().count() << "\n";
    std::cout << "max: " << std::chrono::system_clock::duration::max().count() << "\n";
    return 0;
}
