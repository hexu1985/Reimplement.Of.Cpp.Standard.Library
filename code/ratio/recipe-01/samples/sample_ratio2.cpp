// ratio example
#include <iostream>
#include "ratio.hpp"

int main ()
{
    // mini_stl::ratio<7,-21>::num == -1;
    std::cout << "mini_stl::ratio<7,-21>::num: " << mini_stl::ratio<7,-21>::num << std::endl;
    // mini_stl::ratio<7,-21>::den == 3;
    std::cout << "mini_stl::ratio<7,-21>::den: " << mini_stl::ratio<7,-21>::den << std::endl;
    std::cout << std::endl;

    return 0;
}
