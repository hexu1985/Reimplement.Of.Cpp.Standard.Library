#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include "any.hpp"

class A {
public:
    virtual ~A() {
        std::cout << "A::~A()\n";
    }

    void not_virtual() {
        std::cout << "A::not_virtual()\n";
    }

    virtual void is_virtual () {
        std::cout << "A:: is_virtual ()\n";
    }
};

class B : public A {
public:
    void not_virtual() {
        std::cout << "B::not_virtual()\n";
    }

    virtual void is_virtual () {
        std::cout << "B:: is_virtual ()\n";
    }
};

void foo(Hx::any& a) {
    std::cout << "\n";

    // 试一下 std::shared_ptr<A>
    try {
        std::shared_ptr<A> ptr=
            Hx::any_cast<std::shared_ptr<A> >(a);
        std::cout << "This any contained a std::shared_ptr<A>\n";
        ptr-> is_virtual ();
        ptr->not_virtual();
        return;
    }
    catch(Hx::bad_any_cast& e) {}

    // 试一下 std::shared_ptr<B>
    try {
        std::shared_ptr<B> ptr=
            Hx::any_cast<std::shared_ptr<B> >(a);
        std::cout << "This any contained a std::shared_ptr<B>\n";
        ptr-> is_virtual ();
        ptr->not_virtual();
        return;
    }
    catch(Hx::bad_any_cast& e) {}

    // 如果是其它东西(如一个字符串), 则忽略它
    std::cout <<
        "The any didn't contain anything that \
        concerns this function!\n";
}

int main() {
    std::cout << "Example of any and shared_ptr\n";

    Hx::any a1(std::shared_ptr<A>(new A));
    Hx::any a2(std::string("Just a string"));

    {
        Hx::any b1(std::shared_ptr<A>(new B));
        Hx::any b2(std::shared_ptr<B>(new B));
        std::vector<Hx::any> vec;
        vec.push_back(a1);
        vec.push_back(a2);
        vec.push_back(b1);
        vec.push_back(b2);
        std::for_each(vec.begin(),vec.end(),foo);
        std::cout << "\n";
    }

    std::cout <<
        "any's b1 and b2 have been destroyed which means\n"
        "that the shared_ptrs' reference counts became zero\n";
}
