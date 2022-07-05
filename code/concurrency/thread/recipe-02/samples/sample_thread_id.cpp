#include <iostream>
#include <thread>
#include "thread.hpp"
 
int main()
{
    Hx::thread::id id;
    std::cout << "default thread id: " << id << '\n';

    Hx::thread thread;
    id = thread.get_id();
    std::cout << "norun thread id: " << id << '\n';

    return 0;
}

