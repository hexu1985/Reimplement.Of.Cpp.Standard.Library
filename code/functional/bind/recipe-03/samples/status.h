#ifndef STATUS_INCLUDE
#define STATUS_INCLUDE

#include <string>
#include <iostream>

class status {
  std::string name_;
  bool ok_;
public:
  status(const std::string& name):name_(name),ok_(true) {}

  void break_it() {
    ok_=false;
  }

  bool is_broken() {
    return ok_;
  }

  void report() {
    std::cout << name_ << " is " <<
      (ok_ ? "working nominally":"terribly broken") << '\n';
  }
};

#endif
