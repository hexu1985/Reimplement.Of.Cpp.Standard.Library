#include <iostream>
#include "unique_ptr.hpp"
 
int main() 
{
    const int size = 10; 
    mini_stl::unique_ptr<int[]> fact(new int[size]);
 
    for (int i = 0; i < size; ++i) {
        fact[i] = (i == 0) ? 1 : i * fact[i-1];
    }
 
    for (int i = 0; i < size; ++i) {
        std::cout << i << "! = " << fact[i] << '\n';
    }
}

/*
Output:

0! = 1
1! = 1
2! = 2
3! = 6
4! = 24
5! = 120
6! = 720
7! = 5040
8! = 40320
9! = 362880
*/
