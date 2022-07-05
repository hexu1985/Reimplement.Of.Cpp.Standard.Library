#include <iostream>
#include <functional>
#include "unique_ptr.hpp"
 
struct Foo {
    Foo() { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n\n"; }
};
 
int main()
{
    Foo* foo = new Foo();
    Hx::unique_ptr<Foo> up(foo);
 
    std::cout << "hash(up):  " << std::hash<Hx::unique_ptr<Foo>>()(up) << '\n';
    std::cout << "hash(foo): " << std::hash<Foo*>()(foo) << '\n';
}

/*
Output:

Foo...
hash(up):  3686401041
hash(foo): 3686401041
~Foo...
*/
