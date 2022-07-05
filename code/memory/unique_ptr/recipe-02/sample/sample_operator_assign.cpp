#include <iostream>
#include "unique_ptr.hpp"
 
struct Foo {
    int id;
    Foo(int id) : id(id) { std::cout << "Foo " << id << '\n'; }
    ~Foo() { std::cout << "~Foo " << id << '\n'; }
};
 
int main() 
{
    Hx::unique_ptr<Foo> p1( Hx::make_unique<Foo>(1) );
 
    {
        std::cout << "Creating new Foo...\n";
        Hx::unique_ptr<Foo> p2( Hx::make_unique<Foo>(2) );
        // p1 = p2; // Error ! can't copy unique_ptr
        p1 = std::move(p2);
        std::cout << "About to leave inner block...\n";
 
        // Foo instance will continue to live, 
        // despite p2 going out of scope
    }
 
    std::cout << "About to leave program...\n";
}

/*
Output:

Foo 1
Creating new Foo...
Foo 2
~Foo 1
About to leave inner block...
About to leave program...
~Foo 2
*/
