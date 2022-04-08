// ratio_less example
#include <iostream>
#include <ratio>

int main ()
{
    typedef std::ratio<1,3> one_third;
    typedef std::ratio<1,2> one_half;

    std::cout << "1/3 < 1/2 ? " << std::boolalpha;
    std::cout << std::ratio_less<one_third,one_half>::value << std::endl;

    return 0;
}
