// system_clock::from_time_t
#include <iostream>
#include <ctime>
#include <ratio>
#include "chrono.hpp"

int main ()
{
    using namespace Hx::chrono;

    // create tm with 1/1/2000:
    std::tm timeinfo = std::tm();
    timeinfo.tm_year = 100;   // year: 2000
    timeinfo.tm_mon = 0;      // month: january
    timeinfo.tm_mday = 1;     // day: 1st
    std::time_t tt = std::mktime (&timeinfo);

    system_clock::time_point tp = system_clock::from_time_t (tt);
    system_clock::duration d = system_clock::now() - tp;

    // convert to number of days:
    typedef duration<int,std::ratio<60*60*24>> days_type;
    days_type ndays = duration_cast<days_type> (d);

    // display result:
    std::cout << ndays.count() << " days have passed since 1/1/2000";
    std::cout << std::endl;

    return 0;
}
