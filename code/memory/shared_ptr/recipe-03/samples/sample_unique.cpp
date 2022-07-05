// shared_ptr::unique
#include <iostream>
#include "shared_ptr.hpp"

using Hx::shared_ptr;

int main () {
    shared_ptr<int> foo;
    shared_ptr<int> bar (new int);

    std::cout << "foo unique?\n" << std::boolalpha;

    std::cout << "1: " << foo.unique() << '\n';  // false (empty)

    foo = bar;
    std::cout << "2: " << foo.unique() << '\n';  // false (shared with bar)

    bar = nullptr;
    std::cout << "3: " << foo.unique() << '\n';  // true

    return 0;
}

/*
Output:
foo unique?
1: false
2: false
3: true
*/

