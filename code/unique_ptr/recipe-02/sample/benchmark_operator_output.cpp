#include <iostream>
#include <memory>
 
class Foo {};
 
template <typename charT, typename traits, typename T, typename Deleter>
std::basic_ostream<charT, traits> &operator <<(
        std::basic_ostream<charT, traits> &os, 
        const std::unique_ptr<T, Deleter> &ptr)
{
    os << ptr.get();
    return os;
}

int main()
{
    auto p = std::make_unique<Foo>();
    std::cout << p << '\n';
    std::cout << p.get() << '\n';
}

/*
Possible output:

0x6d9028
0x6d9028
*/
