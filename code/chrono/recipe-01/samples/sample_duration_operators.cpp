// duration operators
#include <iostream>
#include <ratio>
#include "chrono.hpp"

int main ()
{

    mini_stl::chrono::duration<int> foo;
    mini_stl::chrono::duration<int> bar (10);

    // counts: foo bar
    //         --- ---
    foo = bar;                 // 10  10
    foo = foo + bar;           // 20  10
    ++foo;                     // 21  10
    --bar;                     // 21   9
    foo *= 2;                  // 42   9
    foo /= 3;                  // 14   9
    bar += ( foo % bar );      // 14  14

    std::cout << std::boolalpha;
    std::cout << "foo==bar: " << (foo==bar) << std::endl;
    std::cout << "foo: " << foo.count() << std::endl;
    std::cout << "bar: " << bar.count() << std::endl;

    return 0;
}
