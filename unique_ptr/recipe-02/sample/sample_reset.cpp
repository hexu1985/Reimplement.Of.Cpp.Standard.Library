#include <iostream>
#include "unique_ptr.hpp"
 
struct Foo { // object to manage
    Foo() { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n"; }
};
 
struct D { // deleter
    void operator() (Foo* p) {
        std::cout << "Calling delete for Foo object... \n";
        delete p;
    }
};
 
int main()
{
    std::cout << "Creating new Foo...\n";
    mini_stl::unique_ptr<Foo, D> up(new Foo(), D());  // up owns the Foo pointer (deleter D)
 
    std::cout << "Replace owned Foo with a new Foo...\n";
    up.reset(new Foo());  // calls deleter for the old one
 
    std::cout << "Release and delete the owned Foo...\n";
    up.reset(nullptr);      
}

/*
Output:

Creating new Foo...
Foo...
Replace owned Foo with a new Foo...
Foo...
Calling delete for Foo object...
~Foo...
Release and delete the owned Foo...
Calling delete for Foo object...
~Foo...
*/
