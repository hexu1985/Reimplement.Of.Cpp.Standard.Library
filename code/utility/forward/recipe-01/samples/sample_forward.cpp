// forward example
#include "forward.hpp"  // mini_stl::forward
#include <iostream>     // std::cout

// function with lvalue and rvalue reference overloads:
void overloaded (const int& x) {std::cout << "[lvalue]";}
void overloaded (int&& x) {std::cout << "[rvalue]";}

// function template taking rvalue reference to deduced type:
template <class T> void fn (T&& x) {
    overloaded (x);                         // always an lvalue
    overloaded (mini_stl::forward<T>(x));   // rvalue if argument is rvalue
}

int main () {
    int a;

    std::cout << "calling fn with lvalue: ";
    fn (a);
    std::cout << '\n';

    std::cout << "calling fn with rvalue: ";
    fn (0);
    std::cout << '\n';

    return 0;
}
