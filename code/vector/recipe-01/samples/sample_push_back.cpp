#include "vector.hpp"
#include <iostream>
#include <iomanip>
 
int main()
{
    Hx::vector<std::string> letters;
 
    letters.push_back("abc");
    std::string s = "def";
    letters.push_back(std::move(s));
 
    std::cout << "vector holds: ";
#if __cplusplus >= 201402L
    for (auto&& i : letters) std::cout << std::quoted(i) << ' ';
    std::cout << "\nMoved-from string holds " << std::quoted(s) << '\n';
#else
    for (auto&& i : letters) std::cout << i << ' ';
    std::cout << "\nMoved-from string holds " << s << '\n';
#endif
}
