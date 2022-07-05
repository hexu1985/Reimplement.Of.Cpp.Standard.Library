// ratio example
#include <iostream>
#include "ratio.hpp"

int main ()
{
    typedef Hx::ratio<1,3> one_third;
    typedef Hx::ratio<2,4> two_fourths;

    std::cout << "one_third= " << one_third::num << "/" << one_third::den << std::endl;
    std::cout << "two_fourths= " << two_fourths::num << "/" << two_fourths::den << std::endl;

    typedef Hx::ratio_add<one_third,two_fourths> sum;

    std::cout << "sum= " << sum::num << "/" << sum::den;
    std::cout << " (which is: " << ( double(sum::num) / sum::den ) << ")" << std::endl;

    std::cout << "1 kilogram has " << ( Hx::kilo::num / Hx::kilo::den ) << " grams";
    std::cout << std::endl;

    return 0;
}
