/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <vector>
#include "algostuff.hpp"
#include "adjacent_find.hpp"

// return whether the second object has double the value of the first
bool doubled (int elem1, int elem2)
{
    return elem1 * 2 == elem2;
}

int main()
{
    std::vector<int> coll;

    coll.push_back(1);
    coll.push_back(3);
    coll.push_back(2);
    coll.push_back(4);
    coll.push_back(5);
    coll.push_back(5);
    coll.push_back(0);

    PRINT_ELEMENTS(coll,"coll: ");

    // search first two elements with equal value
    std::vector<int>::iterator pos;
    pos = Hx::adjacent_find (coll.begin(), coll.end());

    if (pos != coll.end()) {
        std::cout << "first two elements with equal value have position "
            << std::distance(coll.begin(),pos) + 1
            << std::endl;
    }

    // search first two elements for which the second has double the value of the first
    pos = Hx::adjacent_find (coll.begin(), coll.end(),   // range
            doubled);                   // criterion

    if (pos != coll.end()) {
        std::cout << "first two elements with second value twice the "
            << "first have pos. "
            << std::distance(coll.begin(),pos) + 1
            << std::endl;
    }
}
