#include <iostream>
#include "chrono.hpp"
#include "chrono_literals.hpp"
 
int main()
{
    using namespace mini_stl::chrono_literals;
    auto lesson = 45min;
    auto halfmin = 0.5min;
    std::cout << "one lesson is " << lesson.count() << " minutes\n"
              << "half a minute is " << halfmin.count() << " minutes\n";
}
