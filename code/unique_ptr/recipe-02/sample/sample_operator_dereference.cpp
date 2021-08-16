#include <iostream>
#include "unique_ptr.hpp"
 
struct Foo {
    void bar() { std::cout << "Foo::bar\n"; }
};
 
void f(const Foo& foo) 
{
    std::cout << "f(const Foo&)\n";
}
 
int main() 
{
    mini_stl::unique_ptr<Foo> ptr(new Foo);
 
    ptr->bar();
    f(*ptr);
}

/*
Output:

Foo::bar
f(const Foo&)
*/
