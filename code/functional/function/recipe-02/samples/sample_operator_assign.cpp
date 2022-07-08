// function::operator= example
#include <iostream>     // std::cout
#include <functional>   // std::negate
#include "function.hpp" // Hx::function 

int main () {
    Hx::function<int(int)> foo,bar;
    foo = std::negate<int>();                              // target
    bar = foo;                                             // copy
    foo = Hx::function<int(int)>([](int x){return x+1;});  // move
    bar = nullptr;                                         // clear

    std::cout << "foo: " << foo(100) << '\n';

    return 0;
}
