// get_deleter example
#include <iostream>
#include <memory>

using std::shared_ptr;
using std::get_deleter;

struct D {    // a verbose array deleter:
  void operator()(int* p) {
    std::cout << "[deleter called]\n";
    delete[] p;
  }
};

int main () {
  shared_ptr<int> foo (new int[10],D());

  int * bar = new int[20];

  // use foo's deleter to delete bar (which is unmanaged):
  (*get_deleter<D>(foo))(bar);

  return 0;
  // foo's deleter called automatically
}
