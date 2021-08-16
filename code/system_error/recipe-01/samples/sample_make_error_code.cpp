#include <iostream>
#include <errno.h>
#include "system_error.hpp"   // mini_stl::error_code, std::generic_category

int main()
{
	mini_stl::error_code code;
	mini_stl::error_condition cond;

	code = mini_stl::make_error_code(mini_stl::errc(EPIPE));
	cond = mini_stl::errc(EPIPE);

	std::cout << "code.category: " << code.category().name() << "\n";
	std::cout << "code.message: " << code.message() << "\n";

	std::cout << "cond.category: " << cond.category().name() << "\n";
	std::cout << "cond.message: " << cond.message() << "\n";

	if (code == cond) {
		std::cout << " == \n";
	} else {
		std::cout << " != \n";
	}

	code = mini_stl::error_code(EPIPE, mini_stl::system_category());

	std::cout << "code.category: " << code.category().name() << "\n";
	std::cout << "code.message: " << code.message() << "\n";

	std::cout << "cond.category: " << cond.category().name() << "\n";
	std::cout << "cond.message: " << cond.message() << "\n";

	if (code == cond) {
		std::cout << " == \n";
	} else {
		std::cout << " != \n";
	}

	return 0;
}
