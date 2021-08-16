// shared_ptr relational operators
#include <iostream>
#include <memory>

using std::shared_ptr;
using std::make_shared;

int main () {
	shared_ptr<int> a,b,c,d;

	a = make_shared<int> (10);
	b = make_shared<int> (10);
	c = b;

	std::cout << "comparisons:\n" << std::boolalpha;

	std::cout << "a == b: " << (a==b) << '\n';
	std::cout << "b == c: " << (b==c) << '\n';
	std::cout << "c == d: " << (c==d) << '\n';

	std::cout << "a != nullptr: " << (a!=nullptr) << '\n';
	std::cout << "b != nullptr: " << (b!=nullptr) << '\n';
	std::cout << "c != nullptr: " << (c!=nullptr) << '\n';
	std::cout << "d != nullptr: " << (d!=nullptr) << '\n';

	return 0;
}

/*
Output:
comparisons:
a == b: false
b == c: true
c == d: false
a != nullptr: true
b != nullptr: true
c != nullptr: true
d != nullptr: false
*/

