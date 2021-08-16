#include <iostream>
#include "system_error.hpp"

int main()
{
    try
    {
        throw mini_stl::system_error(EDOM, mini_stl::system_category());
    }
    catch (const mini_stl::system_error& error)
    {
        std::cout << "Error: " << error.code()
                  << " - " << error.code().message() << '\n';
    }
}

/*
Output:

Error: system:33 - Numerical argument out of domain
*/
