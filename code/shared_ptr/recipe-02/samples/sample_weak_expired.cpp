#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using mini_stl::weak_ptr;
using mini_stl::make_shared;

weak_ptr<int> gw;

void f()
{
	if (!gw.expired()) {
		std::cout << "gw is valid\n";
	}
	else {
		std::cout << "gw is expired\n";
	}
}

int main()
{
	{
		auto sp = make_shared<int>(42);
		gw = sp;

		f();
	}

	f();
}

/*
输出：

gw is valid
gw is expired
*/
