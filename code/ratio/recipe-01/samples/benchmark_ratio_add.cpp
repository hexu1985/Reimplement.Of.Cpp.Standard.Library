// ratio_add example
#include <iostream>
#include <ratio>

int main ()
{
    typedef std::ratio<1,2> one_half;
    typedef std::ratio<2,3> two_thirds;

    typedef std::ratio_add<one_half,two_thirds> sum;

    std::cout << "sum = " << sum::num << "/" << sum::den;
    std::cout << " (which is: " << ( double(sum::num) / sum::den ) << ")" << std::endl;

    return 0;
}
