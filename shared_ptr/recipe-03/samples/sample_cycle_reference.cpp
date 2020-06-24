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

struct track
{
	track()
	{ std::cout << "\nconstructing track, this: " << this << "\n"; }

	track(const track &)
	{ std::cout << "\ncopy constructing track, this: " << this << "\n"; }

	track &operator =(const track &)
	{ std::cout << "\ncopying track, this: " << this << "\n"; return *this; }

	~track()
	{ std::cout << "\ndestroying track, this: " << this << "\n"; }
};

struct node: public track
{
    node(int i): value(i) {}

    int value;
    shared_ptr<node> next;
    char pad[32];
};

void print_details(const shared_ptr<node> &sp, const std::string &name)
{
    cout << "-----------------------------\n";
    cout << name << '\n';
    cout << "use_count: " << sp.use_count() << '\n'
        << "get: " << sp.get() << '\n';
    if (sp) {
        cout << "resource value: " << sp->value << '\n';
    } else {
        cout << "no resource\n";
    }
    cout << "-----------------------------\n\n";
}

static void cycle()
{
    cout << __func__ << ":\n";
    shared_ptr<node> head(new node(3));
    shared_ptr<node> N1(new node(4));
    head->next = N1;
    N1->next = head;
    print_details(head, "head");
    print_details(N1, "N1");
}

static void no_cycle()
{
    cout << __func__ << ":\n";
    shared_ptr<node> head(new node(1));
    shared_ptr<node> N1(new node(2));
    head->next = N1;
    print_details(head, "head");
    print_details(N1, "N1");
}

int main()
{
    cycle();

    cout << "\n\n====================================================================\n\n";

    no_cycle();

    return 0;
}

