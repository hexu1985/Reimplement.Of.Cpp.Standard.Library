// duration::count
#include <iostream>     // std::cout
#include "chrono.hpp"   // Hx::chrono::seconds, Hx::chrono::milliseconds
// Hx::chrono::duration_cast

int main ()
{
    using namespace Hx::chrono;
    // Hx::chrono::milliseconds is an instatiation of Hx::chrono::duration:
    milliseconds foo (1000); // 1 second
    foo*=60;

    std::cout << "duration (in periods): ";
    std::cout << foo.count() << " milliseconds.\n";

    std::cout << "duration (in seconds): ";
    std::cout << foo.count() * milliseconds::period::num / milliseconds::period::den;
    std::cout << " seconds.\n";

    return 0;
}
