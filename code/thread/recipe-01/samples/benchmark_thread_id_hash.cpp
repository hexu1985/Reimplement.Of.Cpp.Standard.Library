#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std::chrono_literals;

void foo()
{
    std::this_thread::sleep_for(10ms);
}
 
int main()
{
    std::vector<std::thread> v;
    for(int n = 0; n < 4; ++n)
        v.emplace_back(foo);
 
    std::hash<std::thread::id> hasher;
    for(auto& t : v) {
        std::cout << "thread " << t.get_id() << " hashes to " << hasher(t.get_id()) << '\n';
        t.join();
    }

    return 0;
}
