// -*- C++ -*-
// HeXu's
// 2013 Aug

#ifndef MINI_STL_CHRONO_HIGH_RESOLUTION_CLOCK_INC
#define MINI_STL_CHRONO_HIGH_RESOLUTION_CLOCK_INC

#include "steady_clock.hpp"

namespace mini_stl {

namespace chrono {

/**
 * High resolution clock
 * The members of clock classes provide access to the current time_point.
 * high_resolution_clock is the clock with the shortest tick period. 
 * It may be a synonym for system_clock or steady_clock.
 */
typedef steady_clock high_resolution_clock;

}	// namespace chrono

}	// namespace mini_stl

#endif

