#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"
#include "enable_shared_from_this.hpp"

using Hx::shared_ptr;
using Hx::make_shared;
using Hx::enable_shared_from_this;

struct Foo : public enable_shared_from_this<Foo> {
    Foo() { std::cout << "Foo::Foo\n"; }
    ~Foo() { std::cout << "Foo::~Foo\n"; } 
    shared_ptr<Foo> getFoo() { return shared_from_this(); }
};

int main() {
    Foo *f = new Foo;
    shared_ptr<Foo> pf1;

    {
        shared_ptr<Foo> pf2(f);
        pf1 = pf2->getFoo();  // 与 pf2 的对象共享所有权
    }

    std::cout << "pf2 is gone\n";   
}

/*
输出：

Foo::Foo
pf2 is gone
Foo::~Foo
*/
