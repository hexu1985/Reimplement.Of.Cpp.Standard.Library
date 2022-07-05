#include <iostream>
#include "chrono.hpp"
#include "chrono_literals.hpp"
 
int main()
{
    using namespace Hx::chrono_literals;
    Hx::chrono::seconds halfmin = 30s;
    std::cout << "Half a minute is " << halfmin.count() << " seconds.\n"
              << "A minute and a second is " << (1min + 1s).count() << " seconds.\n";
}
