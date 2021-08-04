#include "set.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <functional>
 
const int nof_operations = 100500;
 
int set_emplace() {
  mini_stl::set<int> set;
  for(int i = 0; i < nof_operations; ++i) {
    set.emplace(i);
  }
  return set.size();
}
 
int set_emplace_hint() {
  mini_stl::set<int> set;
  auto it = set.begin();
  for(int i = 0; i < nof_operations; ++i) {
    set.emplace_hint(it, i);
    it = set.end();
  }
  return set.size();
}
 
int set_emplace_hint_wrong() {
  mini_stl::set<int> set;
  auto it = set.begin();
  for(int i = nof_operations; i > 0; --i) {
    set.emplace_hint(it, i);
    it = set.end();
  }
  return set.size();
}
 
int set_emplace_hint_corrected() {
  mini_stl::set<int> set;
  auto it = set.begin();
  for(int i = nof_operations; i > 0; --i) {
    set.emplace_hint(it, i);
    it = set.begin();
  }
  return set.size();
}
 
int set_emplace_hint_closest() {
  mini_stl::set<int> set;
  auto it = set.begin();
  for(int i = 0; i < nof_operations; ++i) {
    it = set.emplace_hint(it, i);
  }
  return set.size();
}
 
void timeit(std::function<int()> set_test, std::string what = "") {
  auto start = std::chrono::system_clock::now();
  int setsize = set_test();
  auto stop = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> time = stop - start;
  if (what.size() > 0 && setsize > 0) {
    std::cout << std::fixed << std::setprecision(2) << std::setw(5)
              << time.count() << "  ms for " << what << '\n';
  }
}
 
int main() {
   timeit(set_emplace); // stack warmup
   timeit(set_emplace, "plain emplace");
   timeit(set_emplace_hint, "emplace with correct hint");
   timeit(set_emplace_hint_wrong, "emplace with wrong hint");
   timeit(set_emplace_hint_corrected, "corrected emplace");
   timeit(set_emplace_hint_closest, "emplace using returned iterator");
}

/*
Possible output:

18.96  ms for plain emplace
 7.95  ms for emplace with correct hint
19.39  ms for emplace with wrong hint
 8.39  ms for corrected emplace
 7.90  ms for emplace using returned iterator
*/
