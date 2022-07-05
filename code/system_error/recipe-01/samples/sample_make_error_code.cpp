#include <iostream>
#include <errno.h>
#include "system_error.hpp"   // Hx::error_code, std::generic_category

int main()
{
	Hx::error_code code;
	Hx::error_condition cond;

	code = Hx::make_error_code(Hx::errc(EPIPE));
	cond = Hx::errc(EPIPE);

	std::cout << "code.category: " << code.category().name() << "\n";
	std::cout << "code.message: " << code.message() << "\n";

	std::cout << "cond.category: " << cond.category().name() << "\n";
	std::cout << "cond.message: " << cond.message() << "\n";

	if (code == cond) {
		std::cout << " == \n";
	} else {
		std::cout << " != \n";
	}

	code = Hx::error_code(EPIPE, Hx::system_category());

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
