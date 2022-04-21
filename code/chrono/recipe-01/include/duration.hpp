// -*- C++ -*-
// HeXu's
// 2013 Aug

#ifndef MINI_STL_CHRONO_DURATION_INC
#define MINI_STL_CHRONO_DURATION_INC

#include <limits>
#include <type_traits>
#include <ratio>

namespace mini_stl {

namespace chrono {

template <typename Rep, typename Period = std::ratio<1> >
class duration;

namespace detail {

template<typename _Tp>
struct is_ratio: std::false_type {};

template<intmax_t N, intmax_t D>
struct is_ratio<std::ratio<N, D>>: std::true_type {};

template <typename T>
struct is_duration: std::false_type {};

template <typename Rep, typename Period>
struct is_duration<duration<Rep, Period> >: std::true_type {};

template <intmax_t Pn>
struct static_sign: std::integral_constant<intmax_t, (Pn < 0) ? -1 : 1> {};

template <intmax_t Pn>
struct static_abs: std::integral_constant<intmax_t, Pn * static_sign<Pn>::value> {};

template <intmax_t Pn, intmax_t Qn>
struct static_gcd: static_gcd<Qn, (Pn % Qn)> {};

template <intmax_t Pn>
struct static_gcd<Pn, 0>: std::integral_constant<intmax_t, static_abs<Pn>::value> {};

template <intmax_t Qn>
struct static_gcd<0, Qn>: std::integral_constant<intmax_t, static_abs<Qn>::value> {};

template <intmax_t Pn, intmax_t Qn>
struct static_lcm: 
    std::integral_constant<intmax_t, Pn / static_gcd<Pn, Qn>::value * Qn> {};

template <typename R1, typename R2>
struct ratio_gcd: std::ratio<static_gcd<R1::num, R2::num>::value, static_lcm<R1::den, R2::den>::value>::type {};

}    // namespace detail

template <typename Rep>
struct treat_as_floating_point: std::is_floating_point<Rep> {};

/**
 * Duration values
 * This is a traits class to provide the limits and zero value of the type 
 * used to represent the count in a duration object.
 */
template <typename Rep>
struct duration_values {
    static constexpr Rep zero() { return Rep(0); }
    static constexpr Rep min()
    {
        return std::numeric_limits<Rep>::lowest();
    }

    static constexpr Rep max()
    {
        return std::numeric_limits<Rep>::max(); 
    }
};

}    // namespace chrono

}    // namespace mini_stl

namespace std {

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
struct common_type<mini_stl::chrono::duration<Rep1, Period1>, mini_stl::chrono::duration<Rep2, Period2>> {
    typedef mini_stl::chrono::duration<typename std::common_type<Rep1, Rep2>::type, 
            typename mini_stl::chrono::detail::ratio_gcd<Period1, Period2>::type> type;
};

}   // namespace std

namespace mini_stl {

namespace chrono {

template <typename ToDuration, typename Rep, typename Period>
inline constexpr typename
std::enable_if<detail::is_duration<ToDuration>::value, ToDuration>::type
duration_cast(const duration<Rep, Period>& fd);

/**
 * Duration
 * A duration object expresses a time span by means of a count and a period.
 * Internally, the object stores the count as an object of member type rep 
 * (an alias of the first template parameter, Rep), 
 * which can be retrieved by calling member function count.
 * This count is expresed in terms of periods. The length of a period is 
 * integrated in the type (on compile time) by its second template parameter 
 * (Period), which is a ratio type that expresses the number (or fraction) 
 * of seconds that elapse in each period.
 */
template <typename Rep, typename Period>
class duration {
private:
    Rep rep_;

public: 
    typedef Rep rep;
    typedef Period period;

    static_assert(!detail::is_duration<Rep>::value, "rep cannot be a duration");
    static_assert(detail::is_ratio<Period>::value,
              "period must be a specialization of ratio");
    static_assert(Period::num > 0, "period must be positive");

    constexpr duration() = default;
    duration(const duration&) = default;
    ~duration() = default;

    template <typename Rep2, typename = typename
        std::enable_if<std::is_convertible<Rep2, Rep>::value
            && (treat_as_floating_point<Rep>::value
            || !treat_as_floating_point<Rep2>::value)
        >::type>
    constexpr explicit duration(const Rep2& n): rep_(static_cast<Rep>(n)) {}

    template <typename Rep2, typename Period2, typename = typename
        std::enable_if<treat_as_floating_point<Rep>::value
            || (std::ratio_divide<Period2, period>::den == 1
            && !treat_as_floating_point<Rep2>::value)>::type>
    duration(const duration<Rep2, Period2>& dtn):
        rep_(duration_cast<duration>(dtn).count()) {}

    /**
     * Get count
     * Returns the internal count 
     * (i.e., the representation value) of the duration object.
     */
    constexpr rep count() const { return rep_; }

    /** implements unary + and unary - */
    constexpr duration operator+() const { return duration(rep_); }
    constexpr duration operator-() const { return duration(-rep_); }

    /** increments or decrements the tick count */
    duration operator++() { ++rep_; return *this; }
    duration operator++(int) { return duration(rep_++); }
    duration operator--() { --rep_; return *this; }
    duration operator--(int) { return duration(rep_--); }

    /** implements compound assignment between two durations */
    duration& operator+=(const duration& rhs)
    {
        rep_ += rhs.count();
        return *this;
    }

    duration& operator-=(const duration& rhs)
    {
        rep_ -= rhs.count();
        return *this;
    }

    duration& operator*=(const rep& rhs) { rep_ *= rhs; return *this; }
    duration& operator/=(const rep& rhs) { rep_ /= rhs; return *this; }
    duration& operator%=(const rep& rhs) { rep_ %= rhs; return *this; }
    duration& operator%=(const duration& rhs) 
    {
        rep_ %= rhs.count(); 
        return *this; 
    }
    
    /**
     * Zero value
     * Returns a duration value of zero.
     */
    static constexpr duration zero()
    {
        return duration(duration_values<rep>::zero());
    }

    /**
     * Duration minimum value
     * Returns the minimum value of duration.
     */
    static constexpr duration min()
    {
        return duration(duration_values<rep>::min());
    }

    /**
     * Duration maximum value
     * Returns the maximum value of duration.
     */
    static constexpr duration max()
    {
        return duration(duration_values<rep>::max());
    }
};

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
operator+(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    typedef typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type CD;
    return CD(CD(lhs).count()+CD(rhs).count());
}

/**
 * implements arithmetic operations with durations as arguments 
 */
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
operator-(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    typedef typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type CD;
    return CD(CD(lhs).count()-CD(rhs).count());
}

template <typename Rep1, typename Period, typename Rep2>
inline constexpr typename 
std::enable_if<
    std::is_convertible<Rep1, typename std::common_type<Rep1, Rep2>::type>::value &&
    std::is_convertible<Rep1, typename std::common_type<Rep1, Rep2>::type>::value,
    duration<typename std::common_type<Rep1, Rep2>::type, Period>
>::type
operator*(const duration<Rep1, Period>& d, const Rep2& s)
{
    typedef typename std::common_type<Rep1, Rep2>::type CR;
    typedef duration<CR, Period> CD;
    return CD(CD(d).count()*static_cast<CR>(s));
}

template <typename Rep1, typename Rep2, typename Period>
inline constexpr typename
std::enable_if<
    std::is_convertible<Rep1, typename std::common_type<Rep1, Rep2>::type>::value &&
    std::is_convertible<Rep1, typename std::common_type<Rep1, Rep2>::type>::value,
    duration<typename std::common_type<Rep1, Rep2>::type, Period>
>::type
operator*(const Rep1& s, const duration<Rep2, Period>& d)
{
    return d * s;
}

template <typename Rep1, typename Period, typename Rep2>
inline constexpr typename
std::enable_if<
    !detail::is_duration<Rep2>::value,
    duration<typename std::common_type<Rep1, Rep2>::type, Period>
>::type
operator/(const duration<Rep1, Period>& d, const Rep2& s)
{
    typedef typename std::common_type<Rep1, Rep2>::type CR;
    typedef duration<CR, Period> CD;
    return CD(CD(d).count()/static_cast<CR>(s));
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
typename std::common_type<Rep1, Rep2>::type
operator/(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    typedef typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type CD;
    return CD(lhs).count() / CD(rhs).count();
}

template <typename Rep1, typename Period, typename Rep2>
inline constexpr typename
std::enable_if<
    !detail::is_duration<Rep2>::value, 
    duration<typename std::common_type<Rep1, Rep2>::type, Period>
>::type
operator%(const duration<Rep1, Period>& d, const Rep2& s)
{
    typedef typename std::common_type<Rep1, Rep2>::type CR;
    typedef duration<CR, Period> CD;
    return CD(CD(d).count() % static_cast<CR>(s));
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
operator%(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    typedef typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type CD;
    return CD(CD(lhs).count()%CD(rhs).count());
}

namespace detail {

template <typename Duration1, typename Duration2>
struct duration_equal {
    constexpr bool operator()(const Duration1& lhs, const Duration2& rhs)
    {
        typedef typename std::common_type<Duration1, Duration2>::type CD;
        return CD(lhs).count() == CD(rhs).count();
    }
};

template <typename Duration>
struct duration_equal<Duration, Duration> {
    constexpr bool operator()(const Duration& lhs, const Duration& rhs)
    {
        return lhs.count() == rhs.count();
    }
};

}    // namespace detail

/**
 * compares two durations 
 */
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
bool operator==(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    return detail::duration_equal<duration<Rep1, Period1>, 
        duration<Rep2, Period2> >()(lhs, rhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
bool operator!=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    return !(lhs == rhs);
}

namespace detail {

template <typename Duration1, typename Duration2>
struct duration_less {
    constexpr bool operator()(const Duration1& lhs, const Duration2& rhs)
    {
        typedef typename std::common_type<Duration1, Duration2>::type CD;
        return CD(lhs).count() < CD(rhs).count();
    }
};

template <typename Duration>
struct duration_less<Duration, Duration> {
    constexpr bool operator()(const Duration& lhs, const Duration& rhs)
    {
        return lhs.count() < rhs.count();
    }
};

}    // namespace detail

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
bool operator<(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    return detail::duration_less<duration<Rep1, Period1>,
        duration<Rep2, Period2> >()(lhs, rhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
bool operator<=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    return !(rhs < lhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
bool operator>(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    return (rhs < lhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline constexpr
bool operator>=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
    return !(lhs < rhs);
}

namespace detail {

/**
 * duration_cast
 * duration_cast is the heart of this whole prototype. It can convert any
 * duration to any other. It is also (implicitly) used in converting
 * time_points. The conversion is always exact if possible. And it is
 * always as efficient as hand written code. If different representations
 * are involved, care is taken to never require implicit conversions.
 * Instead static_cast is used explicitly for every required conversion.
 * If there are a mixture of integral and floating point representations,
 * the use of common_type ensures that the most logical "intermediate"
 * representation is used.
 */
template <typename FromDuration, typename ToDuration, typename Period, 
    bool PeriodNumEq1, bool PeriodDenEq1>
struct duration_cast_impl;

/**
 * When the two periods are the same, all that is left to do is static_cast 
 * from the source representation to the target representation 
 * (which may be a no-op). This conversion is always exact as long as 
 * the static_cast from the source representation to the destination 
 * representation is exact.
 */
template <typename FromDuration, typename ToDuration, typename Period>
struct duration_cast_impl<FromDuration, ToDuration, Period, true, true>
{
    constexpr ToDuration operator()(const FromDuration& fd) const
    {
        return ToDuration(static_cast<typename ToDuration::rep>(fd.count()));
    }
};

/**
 * When the numerator of FromPeriod / ToPeriod is 1, then all we need to do is
 * divide by the denominator of FromPeriod / ToPeriod. The common_type of
 * the two representations is used for the intermediate computation before
 * static_cast'ing to the destination. This conversion is generally 
 * not exact because of the division (but could be
 * if you get lucky on the run time value of fd.count()).
 */
template <typename FromDuration, typename ToDuration, typename Period>
struct duration_cast_impl<FromDuration, ToDuration, Period, true, false>
{
    constexpr ToDuration operator()(const FromDuration& fd) const
    {
        typedef typename std::common_type<
            typename ToDuration::rep,
            typename FromDuration::rep,
            intmax_t>::type C;
        return ToDuration(static_cast<typename ToDuration::rep>(
            static_cast<C>(fd.count()) / static_cast<C>(Period::den)));
    }
};

/**
 * When the denominator of FromPeriod / ToPeriod is 1, then all we need to do is
 * multiply by the numerator of FromPeriod / ToPeriod.  The common_type of
 * the two representations is used for the intermediate computation before
 * static_cast'ing to the destination. This conversion is always exact 
 * as long as the static_cast's involved are exact.
 */
template <typename FromDuration, typename ToDuration, typename Period>
struct duration_cast_impl<FromDuration, ToDuration, Period, false, true>
{
    constexpr ToDuration operator()(const FromDuration& fd) const
    {
        typedef typename std::common_type<
            typename ToDuration::rep,
            typename FromDuration::rep,
            intmax_t>::type C;
        return ToDuration(static_cast<typename ToDuration::rep>(
            static_cast<C>(fd.count()) * static_cast<C>(Period::num)));
    }
};

/**
 * When neither the numerator or denominator of FromPeriod / ToPeriod is 1, 
 * then we need to multiply by the numerator and divide by the denominator of 
 * FromPeriod / ToPeriod. The common_type of the two representations is used for
 * the intermediate computation before static_cast'ing to the destination.
 * This conversion is generally not exact because of the division 
 * (but could be if you get lucky on the run time value of fd.count()).
 */
template <typename FromDuration, typename ToDuration, typename Period>
struct duration_cast_impl<FromDuration, ToDuration, Period, false, false>
{
    constexpr ToDuration operator()(const FromDuration& fd) const
    {
        typedef typename std::common_type<
            typename ToDuration::rep,
            typename FromDuration::rep,
            intmax_t>::type C;
        return ToDuration(static_cast<typename ToDuration::rep>(
            static_cast<C>(fd.count()) * static_cast<C>(Period::num)
            / static_cast<C>(Period::den)));
    }
};

/**
 * Duration cast
 * Converts the value of dtn into some other duration type, 
 * taking into account differences in their periods.
 * The function does not use implicit conversions. 
 * Instead, all count values are internally converted into 
 * the widest representation (the common_type for the internal count types) 
 * and then casted to the destination type, 
 * all conversions being done explicitly with static_cast.
 * If the destination type has less precision, the value is truncated.
 */
template <typename FromDuration, typename ToDuration>
struct duration_cast {
    typedef typename std::ratio_divide<typename FromDuration::period,
        typename ToDuration::period>::type Period;
    typedef detail::duration_cast_impl<FromDuration, ToDuration,
        Period, Period::num == 1, Period::den == 1> Impl;

    constexpr ToDuration operator() (const FromDuration& fd) const
    {
        return Impl()(fd);
    }
};

}    // namespace detail

template <typename ToDuration, typename Rep, typename Period>
inline constexpr typename
std::enable_if<detail::is_duration<ToDuration>::value, ToDuration>::type
duration_cast(const duration<Rep, Period>& fd)
{
    return detail::duration_cast<duration<Rep, Period>, ToDuration>()(fd);
}

/**
 * Template instantiations
 * The following convenience typedefs of instantiations of 
 * duration are also defined in this namespace:
 * type         Representation                               Period
 * hours        unsigned integral type of at least 23 bits   ratio<3600,1>
 * minutes      unsigned integral type of at least 29 bits   ratio<60,1>
 * seconds      unsigned integral type of at least 35 bits   ratio<1,1>
 * milliseconds unsigned integral type of at least 45 bits   ratio<1,1000>
 * microseconds unsigned integral type of at least 55 bits   ratio<1,1000000>
 * nanoseconds  unsigned integral type of at least 64 bits   ratio<1,1000000000>
 */
typedef duration<int_least32_t, std::ratio<3600> > hours;
typedef duration<int_least32_t, std::ratio<60> > minutes;
typedef duration<int_least64_t> seconds;
typedef duration<int_least64_t, std::milli> milliseconds;
typedef duration<int_least64_t, std::micro> microseconds;
typedef duration<int_least64_t, std::nano> nanoseconds;

}    // namespace chrono

}    // namespace mini_stl

#endif

