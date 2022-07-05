// error_category::name
#include <iostream>			 // std::cout
#include "system_error.hpp" 	 // Hx::error_code

int main() {
	Hx::error_code ec;	// the default error code is system error 0
	std::cout << ec.category().name() << '\n';
	return 0;
} 

/*
Output:

system
*/

