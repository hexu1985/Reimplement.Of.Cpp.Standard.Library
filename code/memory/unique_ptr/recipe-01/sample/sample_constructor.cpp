#include <iostream>
#include "unique_ptr.hpp"
 
struct Foo { // object to manage
    Foo() { std::cout << "Foo ctor\n"; }
    Foo(const Foo&) { std::cout << "Foo copy ctor\n"; }
    Foo(Foo&&) { std::cout << "Foo move ctor\n"; }
    ~Foo() { std::cout << "~Foo dtor\n"; }
};
 
struct D { // deleter
    D() {};
    D(const D&) { std::cout << "D copy ctor\n"; }
    D(D&) { std::cout << "D non-const copy ctor\n";}
    D(D&&) { std::cout << "D move ctor \n"; }
    void operator()(Foo* p) const {
        std::cout << "D is deleting a Foo\n";
        delete p;
    };
};
 
int main()
{
    std::cout << "Example constructor(1)...\n";
    mini_stl::unique_ptr<Foo> up1;  // up1 is empty
    mini_stl::unique_ptr<Foo> up1b(nullptr);  // up1b is empty
 
    std::cout << "Example constructor(2)...\n";
    {
        mini_stl::unique_ptr<Foo> up2(new Foo); //up2 now owns a Foo
    } // Foo deleted
 
    std::cout << "Example constructor(3)...\n";
    D d;
    {  // deleter type is not a reference
       mini_stl::unique_ptr<Foo, D> up3(new Foo, d); // deleter copied
    }
    {  // deleter type is a reference 
       mini_stl::unique_ptr<Foo, D&> up3b(new Foo, d); // up3b holds a reference to d
    }
 
    std::cout << "Example constructor(4)...\n";
    {  // deleter is not a reference 
       mini_stl::unique_ptr<Foo, D> up4(new Foo, D()); // deleter moved
    }
 
    std::cout << "Example constructor(5)...\n";
    {
       mini_stl::unique_ptr<Foo> up5a(new Foo);
       mini_stl::unique_ptr<Foo> up5b(std::move(up5a)); // ownership transfer
    }
 
    std::cout << "Example constructor(6)...\n";
    {
        mini_stl::unique_ptr<Foo, D> up6a(new Foo, d); // D is copied
        mini_stl::unique_ptr<Foo, D> up6b(std::move(up6a)); // D is moved
 
        mini_stl::unique_ptr<Foo, D&> up6c(new Foo, d); // D is a reference
        mini_stl::unique_ptr<Foo, D> up6d(std::move(up6c)); // D is copied
    }
 
#if 0
#if (__cplusplus < 201703L)
    std::cout << "Example constructor(7)...\n";
    {
        std::auto_ptr<Foo> up7a(new Foo);
        mini_stl::unique_ptr<Foo> up7b(std::move(up7a)); // ownership transfer
    }
#endif
#endif
 
#if 0
    std::cout << "Example array constructor...\n";
    {
        mini_stl::unique_ptr<Foo[]> up(new Foo[3]);
    } // three Foo objects deleted
#endif
}

/*
Output:

Example constructor(1)...
Example constructor(2)...
Foo ctor
~Foo dtor
Example constructor(3)...
Foo ctor
D copy ctor
D is deleting a Foo
~Foo dtor
Foo ctor
D is deleting a Foo
~Foo dtor
Example constructor(4)...
Foo ctor
D move ctor 
D is deleting a Foo
~Foo dtor
Example constructor(5)...
Foo ctor
~Foo dtor
Example constructor(6)...
Foo ctor
D copy ctor
D move ctor 
Foo ctor
D non-const copy ctor
D is deleting a Foo
~Foo dtor
D is deleting a Foo
~Foo dtor
Example constructor(7)...
Foo ctor
~Foo dtor
Example array constructor...
Foo ctor
Foo ctor
Foo ctor
~Foo dtor
~Foo dtor
~Foo dtor
*/
