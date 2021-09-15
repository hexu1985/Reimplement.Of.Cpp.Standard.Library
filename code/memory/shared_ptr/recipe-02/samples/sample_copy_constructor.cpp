#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include "shared_ptr.hpp"

using mini_stl::shared_ptr;
using std::cout;

void *operator new(size_t sz)
{
    void *res = malloc(sz);
    if (res == 0) {
        printf("no memory");
        throw std::bad_alloc();
    }
    printf("\n*** new %d bytes at %p ***\n", (int) sz, res);
    return res;
}

void operator delete(void *ptr)
{
    printf("\n*** delete at %p ***\n", ptr);
    free(ptr);
}

void print_details(const shared_ptr<int> &sp, const std::string &name)
{
    cout << "-----------------------------\n";
    cout << name << '\n';
    cout << "use_count: " << sp.use_count() << '\n'
        << "get: " << sp.get() << '\n';
    if (sp) {
        cout << "resource value: " << *sp << '\n';
    } else {
        cout << "no resource\n";
    }
    cout << "-----------------------------\n\n";
}

int main()
{
    cout << "shared_ptr<int> p1(new int(3));" << '\n';
    shared_ptr<int> p1(new int(3));

    print_details(p1, "p1");

    cout << "shared_ptr<int> p2(p1);" << '\n';
    shared_ptr<int> p2(p1);

    print_details(p1, "p1");
    print_details(p2, "p2");

    cout << "p1.reset();" << '\n';
    p1.reset();

    print_details(p1, "p1");
    print_details(p2, "p2");

    cout << "p2.reset();" << '\n';
    p2.reset();

    print_details(p1, "p1");
    print_details(p2, "p2");

    return 0;
}

