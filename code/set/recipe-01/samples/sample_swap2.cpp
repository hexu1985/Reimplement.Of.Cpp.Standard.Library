#include <algorithm>
#include <iostream>
#include "set.hpp"
 
int main()
{
    mini_stl::set<int> alice{1, 2, 3};
    mini_stl::set<int> bob{7, 8, 9, 10};
 
    auto print = [](const int& n) { std::cout << ' ' << n; };
 
    // Print state before swap
    std::cout << "alice:";
    std::for_each(alice.begin(), alice.end(), print);
    std::cout << "\n" "bob  :";
    std::for_each(bob.begin(), bob.end(), print);
    std::cout << '\n';
 
    std::cout << "-- SWAP\n";
    std::swap(alice, bob);
 
    // Print state after swap
    std::cout << "alice:";
    std::for_each(alice.begin(), alice.end(), print);
    std::cout << "\n" "bob  :";
    std::for_each(bob.begin(), bob.end(), print);
    std::cout << '\n';
}

/*
Output:

alice: 1 2 3
bob  : 7 8 9 10
-- SWAP
alice: 7 8 9 10
bob  : 1 2 3
*/
