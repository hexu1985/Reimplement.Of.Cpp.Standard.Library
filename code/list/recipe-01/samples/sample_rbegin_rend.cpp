#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "list.hpp"
 
int main()
{
    Hx::list<int> nums {1, 2, 4, 8, 16};
    Hx::list<std::string> fruits {"orange", "apple", "raspberry"};
    Hx::list<char> empty;
 
    // Print list.
    std::for_each(nums.rbegin(), nums.rend(), [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';
 
    // Sums all integers in the list nums (if any), printing only the result.
    std::cout << "Sum of nums: "
              << std::accumulate(nums.rbegin(), nums.rend(), 0) << '\n';
 
    // Prints the first fruit in the list fruits, checking if there is any.
    if (!fruits.empty())
        std::cout << "First fruit: " << *fruits.rbegin() << '\n';
 
    if (empty.rbegin() == empty.rend())
        std::cout << "list 'empty' is indeed empty.\n";
}
