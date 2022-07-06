#include <functional>
#include <iostream>
#include <type_traits>
 
struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_+i << '\n'; }
    int num_;
};
 
void print_num(int i)
{
    std::cout << i << '\n';
}
 
struct PrintNum {
    void operator()(int i) const
    {
        std::cout << i << '\n';
    }
};
 
int main()
{
    // invoke a free function
    std::invoke(print_num, -9);
 
    // invoke a lambda
    std::invoke([]() { print_num(42); });
 
    // invoke a member function
    const Foo foo(314159);
    std::invoke(&Foo::print_add, foo, 1);
 
    // invoke (access) a data member
    std::cout << "num_: " << std::invoke(&Foo::num_, foo) << '\n';
 
    // invoke a function object
    std::invoke(PrintNum(), 18);
 
#   if defined(__cpp_lib_invoke_r)
    auto add = [](int x, int y) { return x + y; };
    auto ret = std::invoke_r<float>(add, 11, 22);
    static_assert(std::is_same<decltype(ret), float>());
    std::cout << ret << '\n';
    std::invoke_r<void>(print_num, 44);
#   endif
}
