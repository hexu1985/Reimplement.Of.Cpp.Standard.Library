// ratio_subtract example
#include <iostream>
#include "ratio.hpp"

int main ()
{
    typedef mini_stl::ratio<2,3> two_thirds;
    typedef mini_stl::ratio<1,2> one_half;

    typedef mini_stl::ratio_subtract<two_thirds,one_half> diff;

    std::cout << "diff = " << diff::num << "/" << diff::den;
    std::cout << " (which is: " << ( double(diff::num) / diff::den ) << ")" << std::endl;

    return 0;
}
