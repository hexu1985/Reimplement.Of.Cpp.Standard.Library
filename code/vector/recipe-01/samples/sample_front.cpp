#include "vector.hpp"
#include <iostream>
 
int main()
{
    Hx::vector<char> letters {'o', 'm', 'g', 'w', 't', 'f'};
 
    if (!letters.empty()) {
        std::cout << "The first character is '" << letters.front() << "'.\n";
    }  
}
