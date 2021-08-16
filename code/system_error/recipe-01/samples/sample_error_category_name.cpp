// error_category::name
#include <iostream>			 // std::cout
#include "system_error.hpp" 	 // mini_stl::error_code

int main() {
	mini_stl::error_code ec;	// the default error code is system error 0
	std::cout << ec.category().name() << '\n';
	return 0;
} 

/*
Output:

system
*/

