#include <iostream>
#include <string>
#include "set.hpp"
#include <cmath>
 
struct Point { double x, y; };
struct PointCmp {
    bool operator()(const Point& lhs, const Point& rhs) const { 
        return std::hypot(lhs.x, lhs.y) < std::hypot(rhs.x, rhs.y); 
    }
};
 
int main()
{
  // (1) Default constructor
  Hx::set<std::string> a;
  a.insert("cat");
  a.insert("dog");
  a.insert("horse");
  for(auto& str: a) std::cout << str << ' ';
  std::cout << '\n';
 
  // (2) Iterator constructor
  Hx::set<std::string> b(a.find("dog"), a.end());
  for(auto& str: b) std::cout << str << ' ';
  std::cout << '\n';
 
  // (3) Copy constructor
  Hx::set<std::string> c(a);
  c.insert("another horse");
  for(auto& str: c) std::cout << str << ' ';
  std::cout << '\n';
 
  // (4) Move constructor
  Hx::set<std::string> d(std::move(a));
  for(auto& str: d) std::cout << str << ' ';
  std::cout << '\n';
  std::cout << "moved-from set is ";
  for(auto& str: a) std::cout << str << ' ';
  std::cout << '\n';
 
  // (5) Initializer list constructor
  Hx::set<std::string> e {"one", "two", "three", "five", "eight"};
  for(auto& str: e) std::cout << str << ' ';
  std::cout << '\n';
 
  // custom comparison
  Hx::set<Point, PointCmp> z = {{2, 5}, {3, 4}, {1, 1}};
  z.insert({1, -1}); // this fails because the magnitude of 1,-1 equals 1,1
  for(auto& p: z) std::cout << '(' << p.x << ',' << p.y << ") ";
  std::cout << '\n';
}
