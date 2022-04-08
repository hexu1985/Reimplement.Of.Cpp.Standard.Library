// ratio_not_equal example
#include <iostream>
#include "ratio.hpp"

int main ()
{
    typedef mini_stl::ratio<1,2> one_half;
    typedef mini_stl::ratio<2,4> two_fourths;

    std::cout << "1/2 != 2/4 ? " << std::boolalpha;
    std::cout << mini_stl::ratio_not_equal<one_half,two_fourths>::value << std::endl;

    return 0;
}
