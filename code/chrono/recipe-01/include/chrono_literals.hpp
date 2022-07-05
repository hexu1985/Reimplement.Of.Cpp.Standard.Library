// -*- C++ -*-
// HeXu's
// 2022 Apr

#ifndef MINI_STL_CHRONO_LITERALS_INC
#define MINI_STL_CHRONO_LITERALS_INC

#include "duration.hpp"

namespace Hx {

namespace chrono_literals {

constexpr Hx::chrono::hours operator ""h(unsigned long long h)
{
    return Hx::chrono::hours(h);
}

constexpr Hx::chrono::duration<long double, std::ratio<3600,1>> operator ""h(long double h)
{
    return Hx::chrono::duration<long double, std::ratio<3600,1>>(h);
}

constexpr Hx::chrono::minutes operator ""min(unsigned long long m)
{
    return Hx::chrono::minutes(m);
}

constexpr Hx::chrono::duration<long double,
                                std::ratio<60,1>> operator ""min(long double m)
{
    return Hx::chrono::duration<long double, std::ratio<60,1>> (m);
}

constexpr Hx::chrono::seconds operator ""s(unsigned long long s)
{
    return Hx::chrono::seconds(s);
}

constexpr Hx::chrono::duration<long double> operator ""s(long double s)
{
    return Hx::chrono::duration<long double>(s);
}

constexpr Hx::chrono::milliseconds operator ""ms(unsigned long long ms)
{
    return Hx::chrono::milliseconds(ms);
}

constexpr Hx::chrono::duration<long double, std::milli> operator ""ms(long double ms)
{
    return Hx::chrono::duration<long double, std::milli>(ms);
}

constexpr Hx::chrono::microseconds operator ""us(unsigned long long us)
{
    return Hx::chrono::microseconds(us);
}

constexpr Hx::chrono::duration<long double, std::micro> operator ""us(long double us)
{
    return Hx::chrono::duration<long double, std::micro>(us);
}

constexpr Hx::chrono::nanoseconds operator ""ns(unsigned long long ns)
{
    return Hx::chrono::nanoseconds(ns);
}

constexpr Hx::chrono::duration<long double, std::nano> operator ""ns(long double ns)
{
    return Hx::chrono::duration<long double, std::nano>(ns);
}

}   // namespace chrono_literals

}

#endif
