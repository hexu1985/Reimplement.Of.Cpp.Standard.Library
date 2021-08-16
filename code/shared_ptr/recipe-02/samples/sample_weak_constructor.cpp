#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using mini_stl::weak_ptr;
using mini_stl::make_shared;

struct Foo {};

int main()
{
    weak_ptr<Foo> w_ptr;

    {
        auto ptr = make_shared<Foo>();
        w_ptr = ptr;
        std::cout << "w_ptr.use_count() inside scope: " << w_ptr.use_count() << '\n';
    }

    std::cout << "w_ptr.use_count() out of scope: " << w_ptr.use_count() << '\n';
    std::cout << "w_ptr.expired() out of scope: " << std::boolalpha << w_ptr.expired() << '\n';
}

/*
输出：

w_ptr.use_count() inside scope: 1
w_ptr.use_count() out of scope: 0
w_ptr.expired() out of scope: true
*/
