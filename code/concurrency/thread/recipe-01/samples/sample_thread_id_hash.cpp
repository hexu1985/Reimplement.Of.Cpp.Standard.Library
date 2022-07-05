#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "thread.hpp"

using namespace std::chrono;

void foo()
{
    std::this_thread::sleep_for(milliseconds(10));
}
 
int main()
{
    std::vector<mini_stl::thread> v;
    for(int n = 0; n < 4; ++n)
        v.emplace_back(foo);
 
    std::hash<mini_stl::thread::id> hasher;
    for(auto& t : v) {
        std::cout << "thread " << t.get_id() << " hashes to " << hasher(t.get_id()) << '\n';
        t.join();
    }

    return 0;
}
