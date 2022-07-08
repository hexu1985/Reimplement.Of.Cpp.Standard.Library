// function example
#include <iostream>     // std::cout
#include <functional>   // std::function, std::negate

// a function:
int half(int x) {return x/2;}

// a function object class:
struct third_t {
    int operator()(int x) {return x/3;}
};

// a class with data members:
struct MyValue {
    int value;
    int fifth() {return value/5;}
};

int main () {
    std::function<int(int)> fn1 = half;                    // function
    std::function<int(int)> fn2 = &half;                   // function pointer
    std::function<int(int)> fn3 = third_t();               // function object
    std::function<int(int)> fn4 = [](int x){return x/4;};  // lambda expression
    std::function<int(int)> fn5 = std::negate<int>();      // standard function object

    std::cout << "fn1(60): " << fn1(60) << '\n';
    std::cout << "fn2(60): " << fn2(60) << '\n';
    std::cout << "fn3(60): " << fn3(60) << '\n';
    std::cout << "fn4(60): " << fn4(60) << '\n';
    std::cout << "fn5(60): " << fn5(60) << '\n';

    // stuff with members:
    std::function<int(MyValue&)> value = &MyValue::value;  // pointer to data member
    std::function<int(MyValue&)> fifth = &MyValue::fifth;  // pointer to member function

    MyValue sixty {60};

    std::cout << "value(sixty): " << value(sixty) << '\n';
    std::cout << "fifth(sixty): " << fifth(sixty) << '\n';

    return 0;
}
