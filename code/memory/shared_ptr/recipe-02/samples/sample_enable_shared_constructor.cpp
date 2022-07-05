#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"
#include "enable_shared_from_this.hpp"

using Hx::shared_ptr;
using Hx::make_shared;
using Hx::enable_shared_from_this;
 
struct Foo : public enable_shared_from_this<Foo> {
    Foo() {}  // 隐式调用 enable_shared_from_this 构造函数
    shared_ptr<Foo> getFoo() { return shared_from_this(); }
};
 
int main() {
    shared_ptr<Foo> pf1(new Foo);
    auto pf2 = pf1->getFoo();  // 与 pf1 共享对象所有权

	std::cout << "use_count:\n";
	std::cout << "pf1: " << pf1.use_count() << '\n';
	std::cout << "pf2: " << pf2.use_count() << '\n';

    return 0;
}
