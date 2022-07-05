#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include "set.hpp"
#include <string>
 
class Dew
{
  private:
    int a;
    int b;
    int c;
 
  public:
    Dew(int _a, int _b, int _c)
      : a(_a), b(_b), c(_c)
    {}
 
    bool operator<(const Dew &other) const
    {
      if (a < other.a)
        return true;
      if (a == other.a && b < other.b)
        return true;
      return (a == other.a && b == other.b && c < other.c);
    }
};
 
const int nof_operations = 120;
 
int set_emplace() {
    Hx::set<Dew> set;
    for(int i = 0; i < nof_operations; ++i)
        for(int j = 0; j < nof_operations; ++j)
            for(int k = 0; k < nof_operations; ++k)
              set.emplace(i, j, k);
 
    return set.size();
}
 
int set_insert() {
    Hx::set<Dew> set;
    for(int i = 0; i < nof_operations; ++i)
        for(int j = 0; j < nof_operations; ++j)
            for(int k = 0; k < nof_operations; ++k)
              set.insert(Dew(i, j, k));
 
    return set.size();
}
 
void timeit(std::function<int()> set_test, std::string what = "") {
  auto start = std::chrono::system_clock::now();
  int setsize = set_test();
  auto stop = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> time = stop - start;
  if (what.size() > 0 && setsize > 0) {
    std::cout << std::fixed << std::setprecision(2)
        << time.count() << "  ms for " << what << '\n';
  }
}
 
int main()
{
  set_insert();
  timeit(set_insert, "insert");
  timeit(set_emplace, "emplace");
  timeit(set_insert, "insert");
  timeit(set_emplace, "emplace");
}

/*
Possible output:

638.45  ms for insert
619.44  ms for emplace
609.43  ms for insert
652.55  ms for emplace
*/
