#include <iostream>
#include "thread.hpp"
 
int main() 
{
    unsigned int n = mini_stl::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    return 0;
}
