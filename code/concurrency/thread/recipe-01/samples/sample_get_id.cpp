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
    Hx::thread t1(foo);
    Hx::thread::id t1_id = t1.get_id();
 
    Hx::thread t2(foo);
    Hx::thread::id t2_id = t2.get_id();
 
    std::cout << "t1's id: " << t1_id << '\n';
    std::cout << "t2's id: " << t2_id << '\n';
 
    t1.join();
    t2.join();

    return 0;
}
