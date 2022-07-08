#include <vector>
#include <algorithm>
#include <functional>
#include "status.h"

using namespace std::placeholders;

int main() {
  std::vector<status> statuses;
  statuses.push_back(status("status 1"));
  statuses.push_back(status("status 2"));
  statuses.push_back(status("status 3"));
  statuses.push_back(status("status 4"));
  
  statuses[1].break_it();
  statuses[2].break_it();
  
  for (std::vector<status>::iterator it=statuses.begin();
    it!=statuses.end();++it) {
    it->report();
  }

  //============================================
  std::cout << std::string(40, '=') << std::endl;
  std::for_each(  statuses.begin(),  statuses.end(),  std::mem_fun_ref(&status::report));

  //============================================
  std::cout << std::string(40, '=') << std::endl;
  std::for_each(  statuses.begin(),  statuses.end(),  std::bind(&status::report,_1));

  return 0;
}
