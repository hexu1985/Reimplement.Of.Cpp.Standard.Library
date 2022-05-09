#include <iostream>
#include <mutex>
 
int main()
{
    std::recursive_timed_mutex test;
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
