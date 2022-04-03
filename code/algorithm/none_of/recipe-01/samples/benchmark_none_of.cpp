// none_of example
#include <iostream>     // std::cout
#include <algorithm>    // std::none_of
#include <array>        // std::array

int main () {
	std::array<int,8> foo = {1,2,4,8,16,32,64,128};

	if ( std::none_of(foo.begin(), foo.end(), [](int i){return i<0;}) )
		std::cout << "There are no negative elements in the range.\n";

	return 0;
}
