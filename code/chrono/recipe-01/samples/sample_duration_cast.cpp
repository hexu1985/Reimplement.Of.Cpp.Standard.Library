// duration_cast
#include <iostream>     // std::cout
#include "chrono.hpp"   // mini_stl::chrono::seconds, mini_stl::chrono::milliseconds
// mini_stl::chrono::duration_cast

int main ()
{
    mini_stl::chrono::seconds s (1);             // 1 second
    mini_stl::chrono::milliseconds ms = mini_stl::chrono::duration_cast<mini_stl::chrono::milliseconds> (s);

    ms += mini_stl::chrono::milliseconds(2500);  // 2500 millisecond

    s = mini_stl::chrono::duration_cast<mini_stl::chrono::seconds> (ms);   // truncated

    std::cout << "ms: " << ms.count() << std::endl;
    std::cout << "s: " << s.count() << std::endl;

    return 0;
}
