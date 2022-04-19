// duration::zero
#include <iostream>
#include <chrono>

int main ()
{
    using std::chrono::steady_clock;

    steady_clock::time_point t1 = steady_clock::now();

    std::cout << "Printing out something...\n";

    steady_clock::time_point t2 = steady_clock::now();

    steady_clock::duration d = t2 - t1;

    if ( d == steady_clock::duration::zero() )
        std::cout << "The internal clock did not tick.\n";

    else
        std::cout << "The internal clock advanced " << d.count() << " periods.\n";

    return 0;
}
