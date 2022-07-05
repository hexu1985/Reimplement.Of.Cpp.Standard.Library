// enable_shared_from_this example
#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"
#include "enable_shared_from_this.hpp"

struct C : Hx::enable_shared_from_this<C> { };

int main () {
  Hx::shared_ptr<C> foo, bar;

  foo = Hx::make_shared<C>();

  bar = foo->shared_from_this();

  C coo;
  foo = coo.shared_from_this();

  if (!foo.owner_before(bar) && !bar.owner_before(foo))
    std::cout << "foo and bar share ownership";

  return 0;
}
