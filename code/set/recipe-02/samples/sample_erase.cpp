#include "set.hpp"
#include <iostream>

int main()
{
    Hx::set<int> c = {1, 2, 3, 4, 5, 6, 7, 8, 9};
 
    // erase all odd numbers from c
    for(auto it = c.begin(); it != c.end(); ) {
        if(*it % 2 != 0)
            it = c.erase(it);
        else
            ++it;
    }
 
    for(int n : c) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}

/*
Output:

2 4 6 8
*/
