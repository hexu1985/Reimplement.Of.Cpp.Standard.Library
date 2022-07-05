#include <iostream>
#include "system_error.hpp"

int main()
{
    try
    {
        throw Hx::system_error(EDOM, Hx::system_category());
    }
    catch (const Hx::system_error& error)
    {
        std::cout << "Error: " << error.code()
                  << " - " << error.code().message() << '\n';
    }
}

/*
Output:

Error: system:33 - Numerical argument out of domain
*/
