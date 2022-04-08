// ratio example
#include <iostream>
#include "ratio.hpp"

int main ()
{
    typedef mini_stl::ratio<1,3> one_third;
    typedef mini_stl::ratio<2,4> two_fourths;

    std::cout << "one_third= " << one_third::num << "/" << one_third::den << std::endl;
    std::cout << "two_fourths= " << two_fourths::num << "/" << two_fourths::den << std::endl;

    typedef mini_stl::ratio_add<one_third,two_fourths> sum;

    std::cout << "sum= " << sum::num << "/" << sum::den;
    std::cout << " (which is: " << ( double(sum::num) / sum::den ) << ")" << std::endl;

    std::cout << "1 kilogram has " << ( mini_stl::kilo::num / mini_stl::kilo::den ) << " grams";
    std::cout << std::endl;

    return 0;
}
