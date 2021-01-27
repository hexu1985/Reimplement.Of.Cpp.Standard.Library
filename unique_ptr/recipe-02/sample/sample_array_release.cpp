#include <iostream>
#include <cassert>
#include "unique_ptr.hpp"
 
struct Foo {
    Foo() { std::cout << "Foo\n"; }
    ~Foo() { std::cout << "~Foo\n"; }
};
 
int main()
{
    std::cout << "Creating new Foo...\n";
    mini_stl::unique_ptr<Foo[]> up(new Foo[10]);
 
    std::cout << "About to release Foo...\n";
    Foo* fp = up.release();
 
    assert (up.get() == nullptr);
    assert (up == nullptr);
 
    std::cout << "Foo is no longer owned by unique_ptr...\n";
 
    delete [] fp;
}

/*
Output:

Creating new Foo...
Foo
About to release Foo...
Foo is no longer owned by unique_ptr...
~Foo
*/
