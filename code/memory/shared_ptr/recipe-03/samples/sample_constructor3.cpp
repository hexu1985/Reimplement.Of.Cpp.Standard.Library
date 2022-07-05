// shared_ptr constructor example
#include <iostream>
#include <memory>
#include "shared_ptr.hpp"

using Hx::shared_ptr;

struct C {int* data;};

int main () {
    shared_ptr<int> p1;
    shared_ptr<int> p2 (nullptr);
    shared_ptr<int> p3 (new int);
    shared_ptr<int> p4 (new int, std::default_delete<int>());
    shared_ptr<int> p5 (p4);
    shared_ptr<int> p6 (std::move(p5));
    shared_ptr<C> obj (new C);
    shared_ptr<int> p7 (obj, obj->data);

    std::cout << "use_count:\n";
    std::cout << "p1: " << p1.use_count() << '\n';
    std::cout << "p2: " << p2.use_count() << '\n';
    std::cout << "p3: " << p3.use_count() << '\n';
    std::cout << "p4: " << p4.use_count() << '\n';
    std::cout << "p5: " << p5.use_count() << '\n';
    std::cout << "p6: " << p6.use_count() << '\n';
    std::cout << "p7: " << p7.use_count() << '\n';
    return 0;
}

/*
Output:
use_count:
p1: 0
p2: 0
p3: 1
p4: 2
p5: 0
p6: 2
p7: 2
*/
