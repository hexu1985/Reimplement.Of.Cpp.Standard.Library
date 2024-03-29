#include "list.hpp"
#include <iostream>
#include <iomanip>
 
int main()
{
    Hx::list<std::string> letters;
 
    letters.push_back("abc");
    std::string s = "def";
    letters.push_back(std::move(s));
 
    std::cout << "list holds: ";
    for (auto&& i : letters) std::cout << '[' << i << ']' << ' ';
    std::cout << "\nMoved-from string holds " << '[' << s << ']' << '\n';
}
