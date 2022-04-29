#include <iostream>
#include "recursive_mutex.hpp"
 
int main()
{
    mini_stl::recursive_mutex test;
    if (test.try_lock()) {
        std::cout << "lock acquired\n";
        test.unlock();
    } else {
        std::cout << "lock not acquired\n";
    }
 
    test.lock();
    // non-recursive mutex would return false from try_lock now
    if (test.try_lock()) {
        std::cout << "lock acquired\n";
        test.unlock(); 
    } else {
        std::cout << "lock not acquired\n";
    }
    test.unlock();
}
