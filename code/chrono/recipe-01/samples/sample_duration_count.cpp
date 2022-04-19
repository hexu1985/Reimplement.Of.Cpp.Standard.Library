// duration::count
#include <iostream>     // std::cout
#include "chrono.hpp"   // mini_stl::chrono::seconds, mini_stl::chrono::milliseconds
// mini_stl::chrono::duration_cast

int main ()
{
    using namespace mini_stl::chrono;
    // mini_stl::chrono::milliseconds is an instatiation of mini_stl::chrono::duration:
    milliseconds foo (1000); // 1 second
    foo*=60;

    std::cout << "duration (in periods): ";
    std::cout << foo.count() << " milliseconds.\n";

    std::cout << "duration (in seconds): ";
    std::cout << foo.count() * milliseconds::period::num / milliseconds::period::den;
    std::cout << " seconds.\n";

    return 0;
}
