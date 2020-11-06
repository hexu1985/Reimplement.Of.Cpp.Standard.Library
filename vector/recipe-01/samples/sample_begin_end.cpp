#include "vector.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
 
int main()
{
    mini_stl::vector<int> nums {1, 2, 4, 8, 16};
    mini_stl::vector<std::string> fruits {"orange", "apple", "raspberry"};
    mini_stl::vector<char> empty;
 
    // Print vector.
    std::for_each(nums.begin(), nums.end(), [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';
 
    // Sums all integers in the vector nums (if any), printing only the result.
    std::cout << "Sum of nums: "
              << std::accumulate(nums.begin(), nums.end(), 0) << '\n';
 
    // Prints the first fruit in the vector fruits, checking if there is any.
    if (!fruits.empty())
        std::cout << "First fruit: " << *fruits.begin() << '\n';
 
    if (empty.begin() == empty.end())
        std::cout << "vector 'empty' is indeed empty.\n";
}
