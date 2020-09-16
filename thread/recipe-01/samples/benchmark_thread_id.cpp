#include <iostream>
#include <thread>
 
int main()
{
    std::thread::id id;
    std::cout << "default thread id: " << id << '\n';

    std::thread thread;
    id = thread.get_id();
    std::cout << "norun thread id: " << id << '\n';

    return 0;
}

