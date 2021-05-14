#include <iostream>
#include <cerrno>
#include <string>
#include "system_error.hpp"

int main()
{
    mini_stl::error_condition econd = mini_stl::generic_category().default_error_condition(EDOM);
    std::cout << "Category: " << econd.category().name() << '\n'
              << "Value: " << econd.value() << '\n'
              << "Message: " << econd.message() << '\n';
}

/*
Output:

Category: generic
Value: 33
Message: Numerical argument out of domain
*/
