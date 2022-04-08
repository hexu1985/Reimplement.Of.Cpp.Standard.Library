// ratio_greater example
#include <iostream>
#include "ratio.hpp"

int main ()
{
    typedef mini_stl::ratio<1,3> one_third;
    typedef mini_stl::ratio<1,2> one_half;

    std::cout << "1/3 > 1/2 ? " << std::boolalpha;
    std::cout << mini_stl::ratio_greater<one_third,one_half>::value << std::endl;

    return 0;
}
