#include <iostream>
#include "shared_ptr.hpp"

using mini_stl::shared_ptr;

struct Foo {
    Foo() { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n"; }
};

struct D { 
    void operator()(Foo* p) const {
        std::cout << "Call delete from function object...\n";
        delete p;
    }
};

int main()
{
    {
        std::cout << "constructor with no managed object\n";
        shared_ptr<Foo> sh1;
    }

    {
        std::cout << "constructor with object\n";
        shared_ptr<Foo> sh2(new Foo);
        shared_ptr<Foo> sh3(sh2);
        std::cout << sh2.use_count() << '\n';
        std::cout << sh3.use_count() << '\n';
    }

    {
        std::cout << "constructor with object and deleter\n";
        shared_ptr<Foo> sh4(new Foo, D());
        shared_ptr<Foo> sh5(new Foo, [](Foo *p) {
                std::cout << "Call delete from lambda...\n";
                delete p;
                });
    }
}

/*
输出：

constructor with no managed object
constructor with object
Foo...
2
2
~Foo...
constructor with object and deleter
Foo...
Foo...
Call delete from lambda...
~Foo...
Call delete from function object...
~Foo..
*/
