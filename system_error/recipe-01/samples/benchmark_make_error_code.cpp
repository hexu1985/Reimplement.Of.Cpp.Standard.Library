#include <iostream>
#include <errno.h>
#include <system_error>   // std::error_code, std::generic_category

int main()
{
	std::error_code code;
	std::error_condition cond;

	code = std::make_error_code(std::errc(EPIPE));
	cond = std::errc(EPIPE);

	std::cout << "code.category: " << code.category().name() << "\n";
	std::cout << "code.message: " << code.message() << "\n";

	std::cout << "cond.category: " << cond.category().name() << "\n";
	std::cout << "cond.message: " << cond.message() << "\n";

	if (code == cond) {
		std::cout << " == \n";
	} else {
		std::cout << " != \n";
	}

	code = std::error_code(EPIPE, std::system_category());

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
