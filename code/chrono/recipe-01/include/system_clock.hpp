// -*- C++ -*-
// HeXu's
// 2013 Aug

#ifndef MINI_STL_CHRONO_SYSTEM_CLOCK_INC
#define MINI_STL_CHRONO_SYSTEM_CLOCK_INC

#include <time.h>

#include "duration.hpp"
#include "time_point.hpp"

namespace mini_stl {

namespace chrono {

/**
 * System clock
 * Clock classes provide access to the current time_point.
 * Specifically, system_clock is a system-wide realtime clock.
 */
class system_clock {
public:
	typedef nanoseconds duration;
	typedef duration::rep rep;
	typedef duration::period period;
	typedef chrono::time_point<system_clock> time_point;
	static constexpr bool is_steady = false;

	/** Get current time */
	static time_point now() noexcept
	{
		struct timespec ts;
		::clock_gettime(CLOCK_REALTIME, &ts);
		return time_point(duration(
			static_cast<rep>(ts.tv_sec)*1000000000+ts.tv_nsec));
	}
	
	/** Convert to time_t*/
	static time_t to_time_t(const time_point &tp) noexcept
	{
		return tp.time_since_epoch().count() * period::num / period::den;
	}

	/** Convert from time_t */
	static time_point from_time_t(time_t t) noexcept
	{
		return time_point(duration_cast<duration>(seconds(t)));
	}
};

}	// namespace chrono

}	// namespace mini_stl

#endif

