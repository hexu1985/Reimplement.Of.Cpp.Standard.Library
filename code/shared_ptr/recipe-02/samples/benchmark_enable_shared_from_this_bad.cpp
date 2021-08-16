#include <memory>
#include <iostream>

using std::shared_ptr;
using std::make_shared;
using std::enable_shared_from_this;
using std::bad_weak_ptr;

struct Good: enable_shared_from_this<Good> // 注意：继承
{
    shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};

struct Bad
{
    // 错误写法：用不安全的表达式试图获得 this 的 shared_ptr 对象
    shared_ptr<Bad> getptr() {
        return shared_ptr<Bad>(this);
    }
    ~Bad() { std::cout << "Bad::~Bad() called\n"; }
};

int main()
{
    // 正确的示例：两个 shared_ptr 对象将会共享同一对象
    shared_ptr<Good> gp1 = make_shared<Good>();
    shared_ptr<Good> gp2 = gp1->getptr();
    std::cout << "gp2.use_count() = " << gp2.use_count() << '\n';

    // 错误的使用示例：调用 shared_from_this 但其没有被 std::shared_ptr 占有
    try {
        Good not_so_good;
        shared_ptr<Good> gp1 = not_so_good.getptr();
    } catch(bad_weak_ptr& e) {
        // C++17 前为未定义行为； C++17 起抛出 std::bad_weak_ptr 异常
        std::cout << e.what() << '\n';    
    }

    // 错误的示例，每个 shared_ptr 都认为自己是对象仅有的所有者
    shared_ptr<Bad> bp1 = make_shared<Bad>();
    shared_ptr<Bad> bp2 = bp1->getptr();
    std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';
} // UB ： Bad 对象将会被删除两次

/*
可能的输出：

gp2.use_count() = 2
bad_weak_ptr
bp2.use_count() = 1
Bad::~Bad() called
Bad::~Bad() called
*** glibc detected *** ./test: double free or corruption
*/
