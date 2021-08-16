#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "list.hpp"
 
int main()
{
    mini_stl::list<int> nums {1, 2, 4, 8, 16};
    mini_stl::list<std::string> fruits {"orange", "apple", "raspberry"};
    mini_stl::list<char> empty;
 
    // Print list.
    std::for_each(nums.begin(), nums.end(), [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';
 
    // Sums all integers in the list nums (if any), printing only the result.
    std::cout << "Sum of nums: "
              << std::accumulate(nums.begin(), nums.end(), 0) << '\n';
 
    // Prints the first fruit in the list fruits, checking if there is any.
    if (!fruits.empty())
        std::cout << "First fruit: " << *fruits.begin() << '\n';
 
    if (empty.begin() == empty.end())
        std::cout << "list 'empty' is indeed empty.\n";
}
