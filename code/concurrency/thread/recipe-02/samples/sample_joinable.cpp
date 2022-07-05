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
    Hx::thread t;
    std::cout << "before starting, joinable: " << std::boolalpha << t.joinable()
              << '\n';
 
    t = Hx::thread(foo);
    std::cout << "after starting, joinable: " << t.joinable() 
              << '\n';
 
    t.join();
    std::cout << "after joining, joinable: " << t.joinable() 
              << '\n';

    return 0;
}
