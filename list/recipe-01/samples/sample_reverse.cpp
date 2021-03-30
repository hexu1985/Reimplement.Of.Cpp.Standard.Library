#include <iostream>
#include "list.hpp"
 
std::ostream& operator<<(std::ostream& ostr, const mini_stl::list<int>& list)
{
    for (auto &i : list) {
        ostr << " " << i;
    }
    return ostr;
}
 
int main()
{
    mini_stl::list<int> list = { 8,7,5,9,0,1,3,2,6,4 };
 
    std::cout << "before:     " << list << "\n";
    list.sort();
    std::cout << "ascending:  " << list << "\n";
    list.reverse();
    std::cout << "descending: " << list << "\n";
}
