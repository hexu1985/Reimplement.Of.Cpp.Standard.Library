// make_shared example
#include <iostream>
#include <memory>
#include "shared_ptr.hpp"

using Hx::shared_ptr;
using Hx::make_shared;

int main () {

    shared_ptr<int> foo = make_shared<int> (10);
    // same as:
    shared_ptr<int> foo2 (new int(10));

    auto bar = make_shared<int> (20);

    auto baz = make_shared<std::pair<int,int>> (30,40);

    std::cout << "*foo: " << *foo << '\n';
    std::cout << "*bar: " << *bar << '\n';
    std::cout << "*baz: " << baz->first << ' ' << baz->second << '\n';

    return 0;
}
