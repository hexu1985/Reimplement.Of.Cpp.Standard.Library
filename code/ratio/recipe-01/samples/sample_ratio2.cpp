// ratio example
#include <iostream>
#include "ratio.hpp"

int main ()
{
    // Hx::ratio<7,-21>::num == -1;
    std::cout << "Hx::ratio<7,-21>::num: " << Hx::ratio<7,-21>::num << std::endl;
    // Hx::ratio<7,-21>::den == 3;
    std::cout << "Hx::ratio<7,-21>::den: " << Hx::ratio<7,-21>::den << std::endl;
    std::cout << std::endl;

    return 0;
}
