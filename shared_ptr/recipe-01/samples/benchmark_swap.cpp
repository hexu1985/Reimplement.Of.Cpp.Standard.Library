// shared_ptr::swap example
#include <iostream>
#include <memory>

using std::shared_ptr;

int main () {
    shared_ptr<int> foo (new int(10));
    shared_ptr<int> bar (new int(20));

    foo.swap(bar);

    std::cout << "*foo: " << *foo << '\n';
    std::cout << "*bar: " << *bar << '\n';

    return 0;
}

/*
Output:
foo: 20
bar: 10
*/
