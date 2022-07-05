// make_shared example
#include <iostream>
#include <memory>
#include "shared_ptr.hpp"

using Hx::shared_ptr;
using Hx::make_shared;

struct TestClass {
    TestClass(int i) {}

    uint64_t array[10];
};

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

int main () {

    printf("shared_ptr<TestClass> foo = make_shared<TestClass> (10);\n");
    shared_ptr<TestClass> foo = make_shared<TestClass> (10);

    printf("\n\n");
    printf("shared_ptr<TestClass> foo2 (new TestClass(10));\n");
    shared_ptr<TestClass> foo2 (new TestClass(10));

    return 0;
}
