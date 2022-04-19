// -*- C++ -*-
// HeXu's
// 2013 Aug

#ifndef MINI_STL_RATIO_INC
#define MINI_STL_RATIO_INC

#include <cstdint>

namespace mini_stl {

namespace ratio_detail {

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

// Let c = 2^(half # of bits in an intmax_t)
// then we find a1, a0, b1, b0 s.t. N = a1*c + a0, M = b1*c + b0
// The multiplication of N and M becomes,
// N * M = (a1 * b1)c^2 + (a0 * b1 + b0 * a1)c + a0 * b0
// Multiplication is safe if each term and the sum of the terms
// is representable by intmax_t.
template <intmax_t Pn, intmax_t Qn>
struct multiply {
private:
	static constexpr uintmax_t c = uintmax_t(1) << (sizeof(intmax_t) * 4);

	static constexpr uintmax_t a0 = static_abs<Pn>::value % c;
	static constexpr uintmax_t a1 = static_abs<Pn>::value / c;
	static constexpr uintmax_t b0 = static_abs<Qn>::value % c;
	static constexpr uintmax_t b1 = static_abs<Qn>::value / c;

	static_assert(a1 == 0 || b1 == 0, "overflow in multiplication");
	static_assert(a0 * b1 + b0 * a1 < (c >> 1), "overflow in multiplication");
	static_assert(b0 * a0 <= INTMAX_MAX, "overflow in multiplication");
	static_assert(((a0 * b1 + b0 * a1) * c) <= (INTMAX_MAX - b0 * a0),
		"overflow in multiplication");

public:
	static constexpr intmax_t value = Pn * Qn;
};

template <intmax_t Pn, intmax_t Qn, intmax_t Qn_sign = static_sign<Qn>::value>
struct add;

template <intmax_t Pn, intmax_t Qn>
struct add<Pn, Qn, 1> {
private:
	static_assert(Pn <= INTMAX_MAX - Qn, "overflow in add");

public:
	static constexpr intmax_t value = Pn + Qn;
};

template <intmax_t Pn, intmax_t Qn>
struct add<Pn, Qn, 0> {
	static constexpr intmax_t value = Pn;
};

template <intmax_t Pn, intmax_t Qn>
struct add<Pn, Qn, -1> {
private:
	static_assert(Pn >= INTMAX_MIN - Qn, "underflow in add");

public:
	static constexpr intmax_t value = Pn + Qn;
};

template <intmax_t Pn, intmax_t Qn, intmax_t = static_sign<Qn>::value>
struct sub;

template <intmax_t Pn, intmax_t Qn>
struct sub<Pn, Qn, 1> {
private:
	static_assert(Pn >= INTMAX_MIN + Qn, "underflow in sub");

public:
	static constexpr intmax_t value = Pn - Qn;
};

template <intmax_t Pn, intmax_t Qn>
struct sub<Pn, Qn, 0> {
	static constexpr intmax_t value = Pn;
};

template <intmax_t Pn, intmax_t Qn>
struct sub<Pn, Qn, -1> {
private:
	static_assert(Pn <= INTMAX_MAX + Qn, "overflow in sub");

public:
	static constexpr intmax_t value = Pn - Qn;
};

}	// namespace ratio_detail

/**
 * Ratio
 * This template is used to instantiate types that represent 
 * a finite rational number denoted by a numerator and a denominator.
 * The numerator and denominator are implemented as compile-time constants 
 * of type intmax_t.
 */
template <intmax_t N, intmax_t D = 1>
struct ratio {	// N: numerator, D: denominator
    static_assert(D != 0, "denominator cannot be zero");
	static constexpr intmax_t num = 
		N * ratio_detail::static_sign<D>::value / ratio_detail::static_gcd<N, D>::value;
	static constexpr intmax_t den = 
		ratio_detail::static_abs<D>::value / ratio_detail::static_gcd<N, D>::value;
	typedef ratio<num, den> type;
};

/**
 * Multiply two ratios
 * This class template alias generates a ratio type that 
 * is the multiplication of the ratio types R1 and R2.
 */
template <typename R1, typename R2>
struct ratio_multiply {
private:
	static constexpr intmax_t gcd_n1_d2 = 
		ratio_detail::static_gcd<R1::num, R2::den>::value;
	static constexpr intmax_t gcd_n2_d1 = 
		ratio_detail::static_gcd<R2::num, R1::den>::value;

public:
	typedef ratio<
		ratio_detail::multiply<R1::num / gcd_n1_d2, R2::num / gcd_n2_d1>::value,
		ratio_detail::multiply<R1::den / gcd_n2_d1, R2::den / gcd_n1_d2>::value
	> type;

	static constexpr intmax_t num = type::num;
	static constexpr intmax_t den = type::den;
};

/**
 * Divide ratios
 * This class template alias generates a ratio type that 
 * is the result of dividing the ratio type R1 by the ratio type R2.
 */
template <typename R1, typename R2>
struct ratio_divide {
	static_assert(R2::num != 0, "division by 0");

	typedef typename ratio_multiply<R1, ratio<R2::den, R2::num> >::type type;

	static constexpr intmax_t num = type::num;
	static constexpr intmax_t den = type::den;
};

/**
 * Add two ratios
 * This class template alias generates a ratio type that 
 * is the sum of the ratio types R1 and R2.
 */
template <typename R1, typename R2>
struct ratio_add {
private:
	static constexpr intmax_t gcd_n1_n2 = 
		ratio_detail::static_gcd<R1::num, R2::num>::value;
	static constexpr intmax_t gcd_d1_d2 = 
		ratio_detail::static_gcd<R1::den, R2::den>::value;

public:
	typedef typename ratio_multiply<
		ratio<gcd_n1_n2, R1::den / gcd_d1_d2>,
		ratio<
			ratio_detail::add<
				ratio_detail::multiply<
					R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
				ratio_detail::multiply<
					R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
			>::value, R2::den
		>
	>::type type;

	static constexpr intmax_t num = type::num;
	static constexpr intmax_t den = type::den;
};

/**
 * Subtract ratios
 * This class template alias generates a ratio type that 
 * is the difference between the ratio types R1 and R2.
 */
template <typename R1, typename R2>
struct ratio_subtract {
private:
	static constexpr intmax_t gcd_n1_n2 = 
		ratio_detail::static_gcd<R1::num, R2::num>::value;
	static constexpr intmax_t gcd_d1_d2 = 
		ratio_detail::static_gcd<R1::den, R2::den>::value;

public:
	typedef typename ratio_multiply<
		ratio<gcd_n1_n2, R1::den / gcd_d1_d2>,
		ratio<
			ratio_detail::sub<
				ratio_detail::multiply<
					R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
				ratio_detail::multiply<
					R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
			>::value, R2::den
		>
	>::type type;

	static constexpr intmax_t num = type::num;
	static constexpr intmax_t den = type::den;
};

/**
 * Compare ratios
 * This type inherits from the appropriate integral_constant type 
 * (either true_type or false_type) to signal whether R1 compares equal to R2
 */
template <typename R1, typename R2>
struct ratio_equal: 
	std::integral_constant<bool, (R1::num == R2::num && R1::den == R2::den)>
{};

/**
 * This type inherits from the appropriate integral_constant type 
 * (either true_type or false_type) to signal 
 * whether R1 compares not equal to R2
 */
template <typename R1, typename R2>
struct ratio_not_equal: std::integral_constant<bool, !ratio_equal<R1, R2>::value>
{};

namespace ratio_detail {

template <typename R1, typename R2,
	intmax_t Q1 = R1::num / R1::den, intmax_t M1 = R1::num % R1::den,
	intmax_t Q2 = R2::num / R2::den, intmax_t M2 = R2::num % R2::den>
struct ratio_less_impl_1 {
	static constexpr bool value = Q1 < Q2;
};

template <typename R1, typename R2, intmax_t Q>
struct ratio_less_impl_1<R1, R2, Q, 0, Q, 0> {
	static constexpr bool value = false;
};

template <typename R1, typename R2, intmax_t Q, intmax_t M1>
struct ratio_less_impl_1<R1, R2, Q, M1, Q, 0> {
	static constexpr bool value = false;
};

template <typename R1, typename R2, intmax_t Q, intmax_t M2>
struct ratio_less_impl_1<R1, R2, Q, 0, Q, M2> {
	static constexpr bool value = true;
};

template <typename R1, typename R2, intmax_t Q, intmax_t M1, intmax_t M2>
struct ratio_less_impl_1<R1, R2, Q, M1, Q, M2> {
	static constexpr bool value = 
		ratio_less_impl_1<ratio<R2::den, M2>, ratio<R1::den, M1> >::value;
};

template <typename R1, typename R2, 
	intmax_t S1 = static_sign<R1::num>::value, 
	intmax_t S2 = static_sign<R2::num>::value>
struct ratio_less_impl {
	static constexpr bool value = S1 < S2;
};

template <typename R1, typename R2>
struct ratio_less_impl <R1, R2, 1, 1> {
	static constexpr bool value = ratio_less_impl_1<R1, R2>::value;
};

template <typename R1, typename R2>
struct ratio_less_impl <R1, R2, -1, -1> {
	static constexpr bool value = 
		ratio_less_impl_1<ratio<-R2::num, R2::den>,ratio<-R1::num, R1::den> 
		>::value;
};

}	// namespace ratio_detail

/**
 * Compare ratios for less-than inequality
 * This type inherits from the appropriate integral_constant type 
 * (either true_type or false_type) to signal whether R1 compares less than R2
 */
template <typename R1, typename R2>
struct ratio_less: 
	std::integral_constant<bool, ratio_detail::ratio_less_impl<R1, R2>::value> {};

/**
 * Compare ratios for equality or less-than inequality
 * This type inherits from the appropriate integral_constant type 
 * (either true_type or false_type) to signal 
 * whether R1 compares less than or equal to R2
 */
template <typename R1, typename R2>
struct ratio_less_equal: std::integral_constant<bool, !ratio_less<R2, R1>::value> {};

/**
 * Compare ratios for greater than inequality
 * This type inherits from the appropriate integral_constant type 
 * (either true_type or false_type) to signal 
 * whether R1 compares greater than to R2
 */
template <typename R1, typename R2>
struct ratio_greater: std::integral_constant<bool, ratio_less<R2, R1>::value> {};

/**
 * Compare ratios for greater than inequality
 * This type inherits from the appropriate integral_constant type 
 * (either true_type or false_type) to signal 
 * whether R1 compares greater than to R2
 */
template <typename R1, typename R2>
struct ratio_greater_equal: std::integral_constant<bool, !ratio_less<R1, R2>::value>
{};

/**
 * The following predefined standard instantiations of ratio exist:
 * These names match the prefixes used by 
 * standard units of the International System of Units (S.I.).
 */
//typedef ratio<1,1000000000000000000000000> yocto;
//typedef ratio<1,1000000000000000000000> zepto;
typedef ratio<1,1000000000000000000> atto;
typedef ratio<1,1000000000000000> femto;
typedef ratio<1,1000000000000> pico;
typedef ratio<1,1000000000> nano;
typedef ratio<1,1000000> micro;
typedef ratio<1,1000> milli;
typedef ratio<1,100> centi;
typedef ratio<1,10> deci;
typedef ratio<10,1> deca;
typedef ratio<100,1> hecto;
typedef ratio<1000,1> kilo;
typedef ratio<1000000,1> mega;
typedef ratio<1000000000,1> giga;
typedef ratio<1000000000000,1> tera;
typedef ratio<1000000000000000,1> peta;
typedef ratio<1000000000000000000,1> exa;
//typedef ratio<1000000000000000000000,1> zetta;
//typedef ratio<1000000000000000000000000,1> yotta;

template <typename T>
struct is_ratio: public std::false_type {};

template <intmax_t N, intmax_t D>
struct is_ratio<ratio<N, D> >: public std::true_type {};

template <typename R1, typename R2>
struct ratio_gcd: ratio<ratio_detail::static_gcd<R1::num, R2::num>::value, 
	ratio_detail::static_lcm<R1::den, R2::den>::value>::type {};

}	// namespace mini_stl

#endif

