#include <iostream>
#include <string>
#include <functional>
#include "function.hpp"

using namespace std::placeholders;

bool some_function(const std::string& s) {
    std::cout << s << " This is really neat\n";
    return true;
}

class some_class {
public:
    bool some_function(const std::string& s) {
        std::cout << s << " This is also quite nice\n";
        return true;
    }

    bool other_function() {
        std::cout << "other" << " This is also quite nice\n";
        return true;
    }
};

class some_function_object {
public:
    bool operator()(const std::string& s) {
        std::cout << s << 
            " This should work, too, in a flexible solution\n";
        return true;
    }
};

int main() {
    Hx::function<bool (const std::string&)> f1(&some_function);
    f1(std::string("Hello"));

    some_class s;

    std::function<bool (some_class*)> f2(&some_class::other_function);
    f2(&s);

    Hx::function<bool (const std::string&)> f3(std::bind(&some_class::some_function,&s,_1));
    f3(std::string("Hello"));

    some_function_object fso;
    Hx::function<bool (const std::string&)> f4(fso);
    f4(std::string("Hello"));

    return 0;
}

