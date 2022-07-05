#include "vector.hpp"
#include <iostream>
 
int main()
{
    Hx::vector<char> letters {'a', 'b', 'c', 'd', 'e', 'f'};
 
    if (!letters.empty()) {
        std::cout << "The last character is '" << letters.back() << "'.\n";
    }  
}
