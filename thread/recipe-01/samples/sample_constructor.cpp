#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include "thread.hpp"
 
void f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 1 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
void f2(int& n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
class foo
{
public:
    void bar()
    {
        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread 3 executing\n";
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    int n = 0;
};
 
class baz
{
public:
    void operator()()
    {
        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread 4 executing\n";
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    int n = 0;
};
 
int main()
{
    int n = 0;
    foo f;
    baz b;
    mini_stl::thread t1; // t1 不是线程
    mini_stl::thread t2(f1, n + 1); // 按值传递
    mini_stl::thread t3(f2, std::ref(n)); // 按引用传递
    mini_stl::thread t4(std::move(t3)); // t4 现在运行 f2() 。 t3 不再是线程
    mini_stl::thread t5(&foo::bar, &f); // t5 在对象 f 上运行 foo::bar()
    mini_stl::thread t6(std::ref(b)); // t6 在对象 b 上运行 baz::operator()
    t2.join();
    t4.join();
    t5.join();
    t6.join();
    std::cout << "Final value of n is " << n << '\n';
    std::cout << "Final value of foo::n is " << f.n << '\n';
    std::cout << "Final value of baz::n is " << b.n << '\n';
}
