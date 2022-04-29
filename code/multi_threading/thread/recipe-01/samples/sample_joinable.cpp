#include <iostream>
#include <thread>
#include <chrono>
#include "thread.hpp"
 
void foo()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
 
int main()
{
    mini_stl::thread t;
    std::cout << "before starting, joinable: " << std::boolalpha << t.joinable()
              << '\n';
 
    t = mini_stl::thread(foo);
    std::cout << "after starting, joinable: " << t.joinable() 
              << '\n';
 
    t.join();
    std::cout << "after joining, joinable: " << t.joinable() 
              << '\n';

    return 0;
}
