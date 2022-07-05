// shared_ptr::operator->
#include <iostream>
#include "shared_ptr.hpp"

using Hx::shared_ptr;

struct C { int a; int b; };

int main () {
    shared_ptr<C> foo;
    shared_ptr<C> bar (new C);

    foo = bar;

    foo->a = 10;
    bar->b = 20;

    if (foo) std::cout << "foo: " << foo->a << ' ' << foo->b << '\n';
    if (bar) std::cout << "bar: " << bar->a << ' ' << bar->b << '\n';

    return 0;
}

/*
Output:
foo: 10 20
bar: 10 20
*/

