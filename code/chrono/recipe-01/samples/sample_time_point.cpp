// time_point constructors
#include <iostream>
#include <ctime>
#include "chrono.hpp"

int main ()
{
    using namespace mini_stl::chrono;

    system_clock::time_point tp_epoch;	// epoch value

    time_point <system_clock,duration<int>> tp_seconds (duration<int>(1));

    system_clock::time_point tp (tp_seconds);

    std::cout << "1 second since system_clock epoch = ";
    std::cout << tp.time_since_epoch().count();
    std::cout << " system_clock periods." << std::endl;

    // display time_point:
    std::time_t tt = system_clock::to_time_t(tp);
    std::cout << "time_point tp is: " << ctime(&tt);

    return 0;
}
