// ratio_multiply example
#include <iostream>
#include "ratio.hpp"

int main ()
{
    typedef mini_stl::ratio<1,2> one_half;
    typedef mini_stl::ratio<1,3> one_third;

    typedef mini_stl::ratio_multiply<one_half,one_third> result;

    std::cout << "result = " << result::num << "/" << result::den;
    std::cout << " (which is: " << ( double(result::num) / result::den ) << ")" << std::endl;

    return 0;
}
