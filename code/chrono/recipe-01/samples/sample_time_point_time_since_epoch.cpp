// time_point::time_since_epoch
#include <iostream>
#include "chrono.hpp"

int main ()
{
    using namespace mini_stl::chrono;

    system_clock::time_point tp = system_clock::now();
    system_clock::duration dtn = tp.time_since_epoch();

    std::cout << "current time since epoch, expressed in:" << std::endl;
    std::cout << "periods: " << dtn.count() << std::endl;
    std::cout << "seconds: " << dtn.count() * system_clock::period::num / system_clock::period::den;
    std::cout << std::endl;

    return 0;
}
