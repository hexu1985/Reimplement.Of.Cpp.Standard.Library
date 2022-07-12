
#### 引子

学习并掌握一种语言最好的方式, 就是学习这种语言的标准库的使用, 以及学习标准库的实现原理. 本文就是一篇介绍std::function使用和实现的文章.

接下来, 本文将从以下几方面介绍std::funciton

- std::function简介
    + std::function定义
    + std::function如何改进你的程序?
- std::function的用法
    + std::function的声名方式
    + std::function的使用方式
    + std::function作为回调的基础
- std::function的实现

### std::function简介

#### std::function定义

类模板 std::function 是通用多态函数封装器。 std::function 的实例能存储、复制及调用任何可调用 (Callable) 目标——函数、 lambda 表达式、 bind 表达式或其他函数对象，还有指向成员函数指针和指向数据成员指针。

存储的可调用对象被称为 std::function 的目标。若 std::function 不含目标，则称它为空。调用空 std::function 的目标导致抛出 std::bad_function_call 异常。

std::function 满足可复制构造 (CopyConstructible) 和可复制赋值 (CopyAssignable) 。

以上引自: <https://zh.cppreference.com/w/cpp/utility/functional/function>


#### std::function如何改进你的程序?

在进行回调的设计中，常常需要保存函数和函数对象，而且某些函数或类也是通过函数指针或函数对象来配制其客户化功能。通常，函数指针用于实现回调及延时函数。
但是，仅仅使用函数指针会有很多限制，更好的方法是采用泛型机制来定义要被保存的函数的署名特征，而让调用者来决定提供哪一种的类函数实体(函数指针或函数对象)。
这样就可以使用任何行为类似于函数的东西，例如使用std::bind和lambda表达式或其他函数对象。这意味着可以给这些被保存的函数增加状态(因为函数对象是一种类)。
这种泛化由std::function 提供。用于保存并然后调用函数或函数对象。

### std::function的用法

#### std::function的声名方式

一个 function 的声明包括该 function 所兼容的函数或函数对象的签名以及返回类型。结果以及参数的类型以单个参数的方式全部提供给模板。
例如，声明一个 function ，它返回 bool 并接受一个类型 int 的参数，如下：

```cpp
std::function<bool (int)> f;
```

可以在括号中给出参数列表，以逗号分隔，就象普通的函数声明一样。所以，声明一个没有返回值(void)并带有类型分别为 int 和 double 的两个参数的函数，就象这样：

```cpp
std::function<void (int,double)> f;
```

#### std::function的使用方式

理解被存函数的最佳方法是把它想象为一个普通的函数对象，该函数对象用于封装另一个函数(或函数对象)。
这个被存的函数的最大用途是它可以被多次调用，而无须在创建 function 时立即使用。
这里有一个完整的程序，程序声明了一个 std::function ，它可以保存返回 bool (或某个可以隐式转换为 bool 的类型)并接受两个参数的类函数实体，
第一个参数可以转换为 int, 第二个参数可以转换为 double.

```cpp
#include <functional>

bool some_func(int i,double d) {
    return i>d;
}

int main() {
    std::function<bool (int,double)> f;
    f = &some_func;
    f(10,1.1);
}
```

当 function f 首次创建时，它不保存任何函数。它是空的，可以在一个布尔上下文中进行测试。
如果你试图调用一个没有保存任何函数或函数对象的 function ，它将抛出一个类 bad_function_call 的异常。
为了避免这个问题，我们用普通的赋值语法把一个指向 some_func的指针赋值给 f 。这导致 f 保存了到 some_func 的指针。
最后，我们用参数 10 (一个 int) 和 1.1 (一个 double)来调用 f (用函数调用操作符)。
要调用一个 function, 你必须提供被存函数或函数对象所期望的准确数量的参数。


#### 参考资料:

"Beyond the C++ Standard Library: An Introduction to Boost" -- Library 11. Function

