// -*- C++ -*-
// HeXu's
// 2013 Aug

#ifndef MINI_STL_CHRONO_HIGH_RESOLUTION_CLOCK_INC
#define MINI_STL_CHRONO_HIGH_RESOLUTION_CLOCK_INC

#include <time.h>

#include "duration.hpp"
#include "time_point.hpp"

namespace Hx {

namespace chrono {

/**
 * High resolution clock
 * The members of clock classes provide access to the current time_point.
 * high_resolution_clock is the clock with the shortest tick period. 
 * It may be a synonym for system_clock or steady_clock.
 */
class high_resolution_clock {
public:
    typedef nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef chrono::time_point<high_resolution_clock> time_point;
    static constexpr bool is_steady = false;

    /** Get current time */
    static time_point now() noexcept
    {
        struct timespec ts;
        ::clock_gettime(CLOCK_REALTIME, &ts);
        return time_point(duration(
            static_cast<rep>(ts.tv_sec)*1000000000+ts.tv_nsec));
    }
};

}    // namespace chrono

}    // namespace Hx

#endif

