#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using Hx::shared_ptr;
using Hx::weak_ptr;
using Hx::make_shared;

struct Foo {
    int n1;
    int n2; 
    Foo(int a, int b) : n1(a), n2(b) {}
};

int main()
{   
    auto p1 = make_shared<Foo>(1, 2);
    shared_ptr<int> p2(p1, &p1->n1);
    shared_ptr<int> p3(p1, &p1->n2);

    std::cout << std::boolalpha
        << "p2 < p3 " << (p2 < p3) << '\n'
        << "p3 < p2 " << (p3 < p2) << '\n'
        << "p2.owner_before(p3) " << p2.owner_before(p3) << '\n'
        << "p3.owner_before(p2) " << p3.owner_before(p2) << '\n';

    weak_ptr<int> w2(p2);
    weak_ptr<int> w3(p3);
    std::cout 
        //              << "w2 < w3 " << (w2 < w3) << '\n'  // won't compile 
        //              << "w3 < w2 " << (w3 < w2) << '\n'  // won't compile
        << "w2.owner_before(w3) " << w2.owner_before(w3) << '\n'
        << "w3.owner_before(w2) " << w3.owner_before(w2) << '\n';

}

/*
输出：

p2 < p3 true
p3 < p2 false
p2.owner_before(p3) false
p3.owner_before(p2) false
w2.owner_before(w3) false
w3.owner_before(w2) false
*/
