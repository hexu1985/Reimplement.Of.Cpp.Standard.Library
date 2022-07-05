#include <iostream>
#include "thread.hpp"
 
int main() 
{
    unsigned int n = Hx::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    return 0;
}
