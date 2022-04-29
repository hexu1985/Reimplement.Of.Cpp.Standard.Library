#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include "timed_mutex.hpp"
 
mini_stl::timed_mutex test_mutex;
 
void f()
{
    auto now=std::chrono::steady_clock::now();
    test_mutex.try_lock_until(now + std::chrono::seconds(10));
    std::cout << "hello world\n";
}
 
int main()
{
    std::lock_guard<mini_stl::timed_mutex> l(test_mutex);
    std::thread t(f);
    t.join();
}
