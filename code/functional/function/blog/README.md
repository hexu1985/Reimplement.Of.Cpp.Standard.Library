C++STL std::function的使用与实现
================================

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

注: 本文中的示例代码都是基于C++11标准的.

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

#### std::function作为回调的基础

我们先来看看在没有 std::function 以前我们如何实现一个简单的回调，然后再把代码改为使用 function, 并看看会带来什么优势。
我们从一个支持某种简单的回调形式的类开始，它可以向任何对新值关注的对象报告值的改变。这里的回调是一种传统的 C 风格回调，即使用普通函数。
这种回调用可用于象 GUI 控制这样的场合，它可以通知观察者用户改变了它的值，而不需要对监听该信息的客户有任何特殊的知识。

```cpp
#include <iostream>
#include <vector>

void print_new_value(int i) {
  std::cout << 
    "The value has been updated and is now " << i << '\n';
}

void interested_in_the_change(int i) {
  std::cout << "Ah, the value has changed.\n";
}

class notifier {
  typedef void (*function_type)(int);
  std::vector<function_type> vec_;
  int value_;
public:
  void add_observer(function_type t) {
    vec_.push_back(t);
  }

  void change_value(int i) {
    value_=i;
    for (std::size_t i=0;i<vec_.size();++i) {
      (*vec_[i])(value_);
    }
  }
};

int main() {
  notifier n;
  n.add_observer(&print_new_value);
  n.add_observer(&interested_in_the_change);

  n.change_value(42);
}
```

这里的两个函数，print_new_value 和 interested_in_the_change, 它们的函数签名都兼容于 notifier 类的要求。
这些函数指针被保存在一个 vector 内，并且无论何时它的值被改变，这些函数都会在一个循环里被调用。调用这些函数的一种语法是：

```cpp
(*vec_[i])(value_);
```

值(value_)被传递给解引用的函数指针(即 vec_[i] 所返回的)。另一种写法也是有效的，即这样：

```cpp
vec_[i](value_);
```

这种写法看起来更好看些，但更为重要的是，它还可以允许你把函数指针更换为 std::function而没有改变调用的语法。
现在，工作还是正常的，但是，唉，函数对象不能用于这个 notifier 类。
事实上，除了函数指针以外，别的任何东西都不能用，这的确是一种局限。但是，如果我们使用std::function，它就可以工作。
重写这个 notifier 类非常容易。

```cpp
class notifier {
  typedef std::function<void(int)> function_type;
  std::vector<function_type> vec_;
  int value_;
public:
  template <typename T> void add_observer(T t) {
    vec_.push_back(function_type(t));
  }

  void change_value(int i) {
    value_=i;
    for (std::size_t i=0;i<vec_.size();++i) {
      vec_[i](value_);
    }
  }
};
```

首先要做的事是，把 typedef 改为代表 std::function 而不是函数指针。之前，我们定义的是一个函数指针；
现在，我们使用泛型方法，很快就会看到它的用途。接着，我们把成员函数 add_observer 的签名改为泛化的参数类型。
我们也可以把它改为接受一个 std::function，但那样会要求该类的用户必须也知道 function 的使用方法，
而不是仅仅知道这个观察者类型的要求就行了。应该注意到 add_observer 的这种变化并不应该是转向 function 的结果；
无论如何代码应该可以继续工作。我们把它改为泛型的；现在，不管是函数指针、函数对象，还是std::function 实例都可以被传递给 add_observer, 
而无须对已有用户代码进行任何改动。 把元素加入到 vector 的代码有一些修改，现在需要创建一个 std::function<void(int)> 实例。
最后，我们把调用这些函数的语法改为可以使用函数、函数对象以及 std::function 实例。
这种对不同类型的类似函数的"东西"的扩展支持可以立即用于带状态的函数对象，它们可以实现一些用函数很难做到的事情。

```cpp
class knows_the_previous_value {
  int last_value_;
public:
  void operator()(int i) {
    static bool first_time=true;
    if (first_time) {
      last_value_=i;
      std::cout << 
        "This is the first change of value, \
so I don't know the previous one.\n";
      first_time=false;
      return;
    }
    std::cout << "Previous value was " << last_value_ << '\n';
    last_value_=i;
  }
};
```

这个函数对象保存以前的值，并在值被改变时把旧值输出到 std::cout 。注意，当它第一次被调用时，它并不知道旧值。
这个函数对象在函数中使用一个静态 bool 变量来检查这一点，该变量被初始化为 true. 
由于函数中的静态变量是在函数第一次被调用时进行初始化的，所以它仅在第一次调用时被设为 true 。
虽然也可以在普通函数中使用静态变量来提供状态，但是我们必须知道那样不太好，而且很难做到多线程安全。
因此，带状态的函数对象总是优于带静态变量的普通函数。notifier 类并不关心这是不是函数对象，只要符合要求就可以接受。
以下更新的例子示范了它如何使用。

```cpp
int main() {
  notifier n;
  n.add_observer(&print_new_value);
  n.add_observer(&interested_in_the_change);
  n.add_observer(knows_the_previous_value());

  n.change_value(42);
  std::cout << '\n';
  n.change_value(30);
}
```

关键一点要注意的是，我们新增的一个观察者不是函数指针，而是一 knows_the_previous_value 函数对象的实例。运行这段程序的输出如下：

```
The value has been updated and is now 42
Ah, the value has changed.
This is the first change of value, so I don't know the previous one.

The value has been updated and is now 30
Ah, the value has changed.
Previous value was 42
```

修改成std::function的完整示例代码如下:

```cpp
#include <iostream>
#include <vector>
#include <functional>

void print_new_value(int i) {
  std::cout << 
    "The value has been updated and is now " << i << '\n';
}

void interested_in_the_change(int i) {
  std::cout << "Ah, the value has changed.\n";
}

class notifier {
  typedef std::function<void(int)> function_type;
  std::vector<function_type> vec_;
  int value_;
public:
  template <typename T> void add_observer(T t) {
    vec_.push_back(function_type(t));
  }

  void change_value(int i) {
    value_=i;
    for (std::size_t i=0;i<vec_.size();++i) {
      vec_[i](value_);
    }
  }
};

class knows_the_previous_value {
  int last_value_;
public:
  void operator()(int i) {
    static bool first_time=true;
    if (first_time) {
      last_value_=i;
      std::cout << 
        "This is the first change of value, \
so I don't know the previous one.\n";
      first_time=false;
      return;
    }
    std::cout << "Previous value was " << last_value_ << '\n';
    last_value_=i;
  }
};

int main() {
  notifier n;
  n.add_observer(&print_new_value);
  n.add_observer(&interested_in_the_change);
  n.add_observer(knows_the_previous_value());

  n.change_value(42);
  std::cout << '\n';
  n.change_value(30);
}

```

#### 参考资料:

"Beyond the C++ Standard Library: An Introduction to Boost" -- Library 11. Function

