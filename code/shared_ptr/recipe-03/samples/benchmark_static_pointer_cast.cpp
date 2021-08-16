// static_pointer_cast example
#include <iostream>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;

struct A {
    static const char* static_type;
    const char* dynamic_type;
    A() { dynamic_type = static_type; }
};
struct B: A {
    static const char* static_type;
    B() { dynamic_type = static_type; }
};

const char* A::static_type = "class A";
const char* B::static_type = "class B";

int main () {
    shared_ptr<A> foo;
    shared_ptr<B> bar;

    foo = make_shared<A>();

    // cast of potentially incomplete object, but ok as a static cast:
    bar = static_pointer_cast<B>(foo);

    std::cout << "foo's static  type: " << foo->static_type << '\n';
    std::cout << "foo's dynamic type: " << foo->dynamic_type << '\n';
    std::cout << "bar's static  type: " << bar->static_type << '\n';
    std::cout << "bar's dynamic type: " << bar->dynamic_type << '\n';

    return 0;
}


/*
Output:

foo's static  type: class A
foo's dynamic type: class A
bar's static  type: class B
bar's dynamic type: class A
*/
