#include <iostream>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;

struct BaseClass {};

struct DerivedClass : BaseClass
{
    void f() const
    {
        std::cout << "Hello World!\n";
    }
    ~DerivedClass(){ // 注意：它不是虚的
        std::cout << "~DerivedClass\n";
    }
};

int main()
{
    shared_ptr<BaseClass> ptr_to_base(make_shared<DerivedClass>());

    // ptr_to_base->f(); // 错误不会编译： BaseClass 无名为 'f' 的成员

    static_pointer_cast<DerivedClass>(ptr_to_base)->f(); // OK
    // （构造临时 shared_ptr ，然后调用 operator-> ）

    static_cast<DerivedClass*>(ptr_to_base.get())->f(); // 亦 OK
    // （直接转型，不构造临时 shared_ptr ）
}

/*
输出：

Hello World!
Hello World!
~DerivedClass
*/
