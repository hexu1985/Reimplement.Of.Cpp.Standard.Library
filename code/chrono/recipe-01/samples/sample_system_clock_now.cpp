#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include "chrono.hpp"

volatile int sink;
int main()
{
    std::cout << std::fixed << std::setprecision(9) << std::left;
    for (auto size = 1ull; size < 1000000000ull; size *= 100) {
        // record start time
        auto start = Hx::chrono::system_clock::now();
        // do some work
        std::vector<int> v(size, 42);
        sink = std::accumulate(v.begin(), v.end(), 0u); // make sure it's a side effect
        // record end time
        auto end = Hx::chrono::system_clock::now();
        Hx::chrono::duration<double> diff = end - start;
        std::cout << "Time to fill and iterate a vector of " << std::setw(9)
                << size << " ints : " << diff.count() << " s\n";
    }
}
