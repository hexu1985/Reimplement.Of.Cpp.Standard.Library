#include <iostream>
#include "unique_ptr.hpp"
 
struct Foo {
    Foo(int _val) : val(_val) { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n"; }
    int val;
};
 
int main()
{
    mini_stl::unique_ptr<Foo> up1(new Foo(1));
    mini_stl::unique_ptr<Foo> up2(new Foo(2));
 
    up1.swap(up2);
 
    std::cout << "up1->val:" << up1->val << std::endl;
    std::cout << "up2->val:" << up2->val << std::endl;
}

/*
Output:

Foo...
Foo...
up1->val:2
up2->val:1
~Foo...
~Foo...
*/
