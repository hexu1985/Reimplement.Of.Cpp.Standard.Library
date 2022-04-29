#include <iostream>
#include <chrono>
 
int main()
{
    using namespace std::chrono_literals;
    auto lesson = 45min;
    auto halfmin = 0.5min;
    std::cout << "one lesson is " << lesson.count() << " minutes\n"
              << "half a minute is " << halfmin.count() << " minutes\n";
}
