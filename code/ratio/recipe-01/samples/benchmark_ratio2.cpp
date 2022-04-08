// ratio example
#include <iostream>
#include <ratio>

int main ()
{
    // std::ratio<7,-21>::num == -1;
    std::cout << "std::ratio<7,-21>::num: " << std::ratio<7,-21>::num << std::endl;
    // std::ratio<7,-21>::den == 3;
    std::cout << "std::ratio<7,-21>::den: " << std::ratio<7,-21>::den << std::endl;
    std::cout << std::endl;

    return 0;
}
