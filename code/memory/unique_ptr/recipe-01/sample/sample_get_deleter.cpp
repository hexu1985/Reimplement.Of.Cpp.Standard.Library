#include <iostream>
#include <memory>
#include "unique_ptr.hpp"
 
struct Foo
{
    Foo() { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n"; }
};
 
struct D
{
    void bar() { std::cout << "Call deleter D::bar()...\n"; }
    void operator()(Foo* p) const
    {
        std::cout << "Call delete for Foo object...\n";
        delete p;
    }
};
 
int main()
{
    Hx::unique_ptr<Foo, D> up(new Foo(), D());
    D& del = up.get_deleter();
    del.bar();
}

/*
Output:

Foo...
Call deleter D::bar()...
Call delete for Foo object...
~Foo...
*/
