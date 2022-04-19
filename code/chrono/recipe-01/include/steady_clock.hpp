// -*- C++ -*-
// HeXu's
// 2013 Aug

#ifndef MINI_STL_CHRONO_STEADY_CLOCK_INC
#define MINI_STL_CHRONO_STEADY_CLOCK_INC

#include <time.h>
#include "duration.hpp"
#include "time_point.hpp"

namespace mini_stl {

namespace chrono {

/**
 * Steady clock
 * Clock classes provide access to the current time_point.
 * steady_clock is specifically designed to calculate time intervals.
 */
class steady_clock {
public:
	typedef nanoseconds duration;
	typedef duration::rep rep;
	typedef duration::period period;
	typedef chrono::time_point<steady_clock> time_point;
	static constexpr bool is_steady = true;

	/** Get current time */
	static time_point now() noexcept
	{
		struct timespec ts;
		::clock_gettime(CLOCK_MONOTONIC, &ts);
		return time_point(duration(
			static_cast<rep>(ts.tv_sec)*1000000000+ts.tv_nsec));
	}
};
	
}	// namespace chrono

}	// namespace mini_stl

#endif

