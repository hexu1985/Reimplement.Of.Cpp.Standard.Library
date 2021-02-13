#include <vector>
#include <iostream>
 
int main()
{
    std::vector<char> letters {'a', 'b', 'c', 'd', 'e', 'f'};
 
    if (!letters.empty()) {
        std::cout << "The last character is '" << letters.back() << "'.\n";
    }  
}
