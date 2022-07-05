// duration_cast
#include <iostream>     // std::cout
#include "chrono.hpp"   // Hx::chrono::seconds, Hx::chrono::milliseconds
// Hx::chrono::duration_cast

int main ()
{
    Hx::chrono::seconds s (1);             // 1 second
    Hx::chrono::milliseconds ms = Hx::chrono::duration_cast<Hx::chrono::milliseconds> (s);

    ms += Hx::chrono::milliseconds(2500);  // 2500 millisecond

    s = Hx::chrono::duration_cast<Hx::chrono::seconds> (ms);   // truncated

    std::cout << "ms: " << ms.count() << std::endl;
    std::cout << "s: " << s.count() << std::endl;

    return 0;
}
