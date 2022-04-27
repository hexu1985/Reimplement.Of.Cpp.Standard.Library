// steady_clock example
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

int main ()
{
    using namespace std::chrono;

    steady_clock::time_point clock_begin = steady_clock::now();

    std::cout << "printing out 1000 stars...\n";
    for (int i=0; i<1000; ++i) std::cout << "*";
    std::cout << std::endl;

    steady_clock::time_point clock_end = steady_clock::now();

    steady_clock::duration time_span = clock_end - clock_begin;

    double nseconds = double(time_span.count()) * steady_clock::period::num / steady_clock::period::den;

    std::cout << "It took me " << nseconds << " seconds.";
    std::cout << std::endl;

    return 0;
}
