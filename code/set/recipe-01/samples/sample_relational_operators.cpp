#include <iostream>
#include "set.hpp"
 
int main()
{
    mini_stl::set<int> alice{1, 2, 3};
    mini_stl::set<int> bob{7, 8, 9, 10};
    mini_stl::set<int> eve{1, 2, 3};
 
    std::cout << std::boolalpha;
 
    // Compare non equal containers
    std::cout << "alice == bob returns " << (alice == bob) << '\n';
    std::cout << "alice != bob returns " << (alice != bob) << '\n';
    std::cout << "alice <  bob returns " << (alice < bob) << '\n';
    std::cout << "alice <= bob returns " << (alice <= bob) << '\n';
    std::cout << "alice >  bob returns " << (alice > bob) << '\n';
    std::cout << "alice >= bob returns " << (alice >= bob) << '\n';
 
    std::cout << '\n';
 
    // Compare equal containers
    std::cout << "alice == eve returns " << (alice == eve) << '\n';
    std::cout << "alice != eve returns " << (alice != eve) << '\n';
    std::cout << "alice <  eve returns " << (alice < eve) << '\n';
    std::cout << "alice <= eve returns " << (alice <= eve) << '\n';
    std::cout << "alice >  eve returns " << (alice > eve) << '\n';
    std::cout << "alice >= eve returns " << (alice >= eve) << '\n';
}

/*
Output:

alice == bob returns false
alice != bob returns true
alice <  bob returns true
alice <= bob returns true
alice >  bob returns false
alice >= bob returns false
 
alice == eve returns true
alice != eve returns false
alice <  eve returns false
alice <= eve returns true
alice >  eve returns false
alice >= eve returns true
*/
