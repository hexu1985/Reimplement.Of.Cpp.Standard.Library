#include <list>
#include <iostream>
 
int main()
{
    std::list<char> letters {'a', 'b', 'c', 'd', 'e', 'f'};
 
    if (!letters.empty()) {
        std::cout << "The last character is '" << letters.back() << "'.\n";
    }  
}
