#include <iostream>
#include <vector>
#include <string>
#include "any.hpp"

int main()
{
    std::vector<Hx::any> some_values;
    some_values.push_back(10);
    const char* c_str = "Hello there!";
    some_values.push_back(c_str);
    some_values.push_back(std::string("Wow!"));
    std::string& s =
        Hx::any_cast<std::string&>(some_values.back());
    s += " That is great!\n";
    std::cout << s;
    return 0;
}
