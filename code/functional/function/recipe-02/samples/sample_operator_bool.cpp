#include <iostream>
#include "function.hpp"

void sampleFunction()
{
    std::cout << "This is the sample function!\n";
}

void checkFunc( Hx::function<void()> const &func )
{
    // Use operator bool to determine if callable target is available.
    if( func )  
    {
        std::cout << "Function is not empty! Calling function.\n";
        func();
    }
    else
    {
        std::cout << "Function is empty. Nothing to do.\n";
    }
}

int main()
{
    Hx::function<void()> f1;
    Hx::function<void()> f2( sampleFunction );

    std::cout << "f1: ";
    checkFunc( f1 );

    std::cout << "f2: ";
    checkFunc( f2 );
}
