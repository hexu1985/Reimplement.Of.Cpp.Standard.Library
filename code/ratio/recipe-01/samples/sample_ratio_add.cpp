// ratio_add example
#include <iostream>
#include "ratio.hpp"

int main ()
{
    typedef mini_stl::ratio<1,2> one_half;
    typedef mini_stl::ratio<2,3> two_thirds;

    typedef mini_stl::ratio_add<one_half,two_thirds> sum;

    std::cout << "sum = " << sum::num << "/" << sum::den;
    std::cout << " (which is: " << ( double(sum::num) / sum::den ) << ")" << std::endl;

    return 0;
}
