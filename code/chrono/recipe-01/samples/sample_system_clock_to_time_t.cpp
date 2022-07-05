#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include "chrono.hpp"

int main()
{
    // The old way
    std::time_t oldt = std::time(nullptr);

    std::this_thread::sleep_for(std::chrono::milliseconds(2700));

    // The new way
    std::time_t newt = Hx::chrono::system_clock::to_time_t(
                            Hx::chrono::system_clock::now());

    std::cout << "oldt-newt == " << oldt-newt << " s\n";
}
