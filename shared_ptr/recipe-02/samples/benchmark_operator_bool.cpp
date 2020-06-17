// example of shared_ptr::operator bool
#include <iostream>
#include <memory>

using std::shared_ptr;

int main () {
	shared_ptr<int> foo;
	shared_ptr<int> bar (new int(34));

	if (foo) std::cout << "foo points to " << *foo << '\n';
	else std::cout << "foo is null\n";

	if (bar) std::cout << "bar points to " << *bar << '\n';
	else std::cout << "bar is null\n";

	return 0;
}

/*
Output:
foo is null
bar points to 34
*/

