// -*- C++ -*-
// HeXu's
// 2013 Aug

#ifndef MINI_STL_CHRONO_TIME_POINT_INC
#define MINI_STL_CHRONO_TIME_POINT_INC

#include "duration.hpp"

namespace mini_stl {

namespace chrono {

template <typename Clock, typename Duration = typename Clock::duration>
class time_point;

}	// namespace chrono

}	// namespace mini_stl

namespace std {

template <typename Clock, typename Duration1, typename Duration2>
struct common_type<mini_stl::chrono::time_point<Clock, Duration1>,
	mini_stl::chrono::time_point<Clock, Duration2>> {
	typedef mini_stl::chrono::time_point<Clock,
		typename common_type<Duration1, Duration2>::type> type;
};

}	// namespace std

namespace mini_stl {

namespace chrono {

template <typename Clock, typename Duration>
class time_point {
private:
	Duration d_;

public:
	typedef Clock clock;	// The clock class. 
	typedef Duration duration;	// The duration type. 
	typedef typename duration::rep rep;	// Type returned by duration::count.
	typedef typename duration::period period;	
	// The ratio type that represents the length of a period in seconds.

	/**
	 * default constructor
	 * Constructs an object with the epoch as value.
	 */
	constexpr time_point(): d_(duration::zero()) {}

	/**
	 * Constructs an object representing a time point 
	 * where a duration of d has elapsed since the epoch.
	 */
	constexpr explicit time_point(const duration& d): d_(d) {}

	/**
	 * Constructs an object representing the same time point as tp.
	 * Only called if Duration2 is implicitly convertible 
	 * to the duration type of the newly constructed object.
	 */
	template <typename Duration2>
	constexpr time_point(const time_point<clock, Duration2>& tp):
		d_(tp.time_since_epoch()) {}

	/*
	 * Time since epoch
	 * Returns a duration object with the time span value 
	 * between the epoch and the time point.
	 * The value returned is the current value of the internal duration object.
	 */
	constexpr duration time_since_epoch() const { return d_; }

	/** modifies the time point by the given duration */
	time_point& operator+=(const duration& d)
	{
		d_ += d;
		return *this;
	}

	time_point& operator-=(const duration& d)
	{
		d_ -= d;
		return *this;
	}

	/**
	 * Minimum value of time_point
	 * Returns the minimum value of time_point.
	 */
	static constexpr time_point min() { return time_point(duration::min()); }

	/**
	 * Maximum value of time_point
	 * Returns the maximum value of time_point.
	 */
	static constexpr time_point max() { return time_point(duration::max()); }
};

/** modifies the time point by the given duration */ 
template <typename Clock, typename Duration1, typename Rep2, typename Period2>
inline constexpr
time_point<Clock, typename 
	std::common_type<Duration1, duration<Rep2, Period2>>::type>
operator+(const time_point<Clock, Duration1>& tp, const duration<Rep2, Period2>& d)
{
	typedef typename 
		std::common_type<Duration1, duration<Rep2, Period2>>::type CDuration;
	return time_point<Clock, CDuration>(tp.time_since_epoch() + d);
}

template <typename Rep1, typename Period1, typename Clock, typename Duration2>
inline constexpr
time_point<Clock, typename 
	std::common_type<duration<Rep1, Period1>, Duration2>::type>
operator+(const duration<Rep1, Period1>& d, const time_point<Clock, Duration2>& tp)
{
	return (tp + d);
}

template <typename Clock, typename Duration1, typename Rep2, typename Period2>
inline constexpr
time_point<Clock, typename 
	std::common_type<Duration1, duration<Rep2, Period2>>::type>
operator-(const time_point<Clock, Duration1>& tp, const duration<Rep2, Period2>& d)
{
	typedef typename 
		std::common_type<Duration1, duration<Rep2, Period2>>::type CDuration;
	return time_point<Clock, CDuration>(tp.time_since_epoch() - d);
}

template <typename Clock, typename Duration1, typename Duration2>
inline constexpr typename
std::common_type<Duration1, Duration2>::type
operator-(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs)
{
	return lhs.time_since_epoch() - rhs.time_since_epoch();
}

/** compares two time points */
template <typename Clock, typename Duration1, typename Duration2>
inline constexpr 
bool operator==(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs)
{
	return lhs.time_since_epoch() == rhs.time_since_epoch();
}

template <typename Clock, typename Duration1, typename Duration2>
inline constexpr 
bool operator!=(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs)
{
	return (lhs != rhs);
}

template <typename Clock, typename Duration1, typename Duration2>
inline constexpr 
bool operator<(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs)
{
	return lhs.time_since_epoch() < rhs.time_since_epoch();
}

template <typename Clock, typename Duration1, typename Duration2>
inline constexpr 
bool operator>(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs)
{
	return (rhs < lhs);
}

template <typename Clock, typename Duration1, typename Duration2>
inline constexpr 
bool operator<=(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs)
{
	return !(rhs < lhs);
}

template <typename Clock, typename Duration1, typename Duration2>
inline constexpr 
bool operator>=(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs)
{
	return !(lhs < rhs);
}

/**
 * Time_point cast
 * Converts the value of tp into a time_point type with 
 * a different duration internal object, taking into account 
 * differences in their durations's periods.
 */
template <typename ToDuration, typename Clock, typename Duration>
inline constexpr typename 
std::enable_if<
	detail::is_duration<ToDuration>::value, time_point<Clock, ToDuration> 
>::type time_point_cast(const time_point<Clock, Duration> &tp)
{
	return time_point<Clock, ToDuration>(
		duration_cast<ToDuration>(tp.time_since_epoch()));
}

}	// namespace chrono

}	// namespace mini_stl

#endif
