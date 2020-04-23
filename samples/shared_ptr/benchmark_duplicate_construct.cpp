#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <memory>

using std::shared_ptr;
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
    int *p = new int(3);
    shared_ptr<int> sp0(p);
    shared_ptr<int> sp1(p);
    print_details(sp0, "sp0");
    print_details(sp1, "sp1");

    sp0.reset();
    sp1.reset();

    return 0;
}
