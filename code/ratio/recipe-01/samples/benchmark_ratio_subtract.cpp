// ratio_subtract example
#include <iostream>
#include <ratio>

int main ()
{
    typedef std::ratio<2,3> two_thirds;
    typedef std::ratio<1,2> one_half;

    typedef std::ratio_subtract<two_thirds,one_half> diff;

    std::cout << "diff = " << diff::num << "/" << diff::den;
    std::cout << " (which is: " << ( double(diff::num) / diff::den ) << ")" << std::endl;

    return 0;
}
