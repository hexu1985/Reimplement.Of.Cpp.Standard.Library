#include <iostream>
#include <system_error>

int main()
{
    try
    {
        throw std::system_error(EDOM, std::system_category());
    }
    catch (const std::system_error& error)
    {
        std::cout << "Error: " << error.code()
                  << " - " << error.code().message() << '\n';
    }
}

/*
Output:

Error: system:33 - Numerical argument out of domain
*/
