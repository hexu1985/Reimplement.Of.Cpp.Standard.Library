#include "set.hpp"
#include <iostream>
 
int main()
{ 
    mini_stl::set<int> nums {1, 3, 5, 7};
 
    std::cout << "nums contains " << nums.size() << " elements.\n";
}

/*
Output:

nums contains 4 elements.
*/
