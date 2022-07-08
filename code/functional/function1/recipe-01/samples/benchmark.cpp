#include <iostream>
#include <string>
#include <functional>

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
    std::function<bool (const std::string&)> f1(&some_function);
    f1(std::string("Hello"));

    some_class s;

#if 0
    std::function<bool (const std::string&)> f2(&some_class::some_function,&s);
    f2(std::string("Hello"));
#endif

    std::function<bool (const std::string&)> f3(std::bind(&some_class::some_function,&s,_1));
    f3(std::string("Hello"));

    some_function_object fso;
    std::function<bool (const std::string&)> f4(fso);
    f4(std::string("Hello"));

    return 0;
}

