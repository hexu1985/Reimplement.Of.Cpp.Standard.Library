#include <iostream>
#include <tuple>
#include "make_from_tuple.hpp"
 
struct Foo {
    Foo(int first, float second, int third) {
        std::cout << first << ", " << second << ", " << third << "\n";
    }
};
 
int main()
{
   auto tuple = std::make_tuple(42, 3.14f, 0);
   Hx::make_from_tuple<Foo>(std::move(tuple));
}
