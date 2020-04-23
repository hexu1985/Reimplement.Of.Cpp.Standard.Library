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
        << "get: " << sp.get() << '\n'
        << "operator bool: " << static_cast<bool>(sp) << '\n';
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

    cout << "shared_ptr<int> p2(new int(4));" << '\n';
    shared_ptr<int> p2(new int(4));

    print_details(p1, "p1");
    print_details(p2, "p2");

    cout << "p1 = p2;" << '\n';
    p1 = p2;

    print_details(p1, "p1");
    print_details(p2, "p2");

    return 0;
}

