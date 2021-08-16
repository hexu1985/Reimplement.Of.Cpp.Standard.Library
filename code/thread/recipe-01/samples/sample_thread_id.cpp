#include <iostream>
#include <thread>
#include "thread.hpp"
 
int main()
{
    mini_stl::thread::id id;
    std::cout << "default thread id: " << id << '\n';

    mini_stl::thread thread;
    id = thread.get_id();
    std::cout << "norun thread id: " << id << '\n';

    return 0;
}

