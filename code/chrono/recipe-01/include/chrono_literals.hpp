// -*- C++ -*-
// HeXu's
// 2022 Apr

#ifndef MINI_STL_CHRONO_LITERALS_INC
#define MINI_STL_CHRONO_LITERALS_INC

#include "duration.hpp"

namespace mini_stl {

namespace chrono_literals {

constexpr mini_stl::chrono::hours operator ""h(unsigned long long h)
{
    return mini_stl::chrono::hours(h);
}

constexpr mini_stl::chrono::duration<long double, std::ratio<3600,1>> operator ""h(long double h)
{
    return mini_stl::chrono::duration<long double, std::ratio<3600,1>>(h);
}

constexpr mini_stl::chrono::minutes operator ""min(unsigned long long m)
{
    return mini_stl::chrono::minutes(m);
}

constexpr mini_stl::chrono::duration<long double,
                                std::ratio<60,1>> operator ""min(long double m)
{
    return mini_stl::chrono::duration<long double, std::ratio<60,1>> (m);
}

constexpr mini_stl::chrono::seconds operator ""s(unsigned long long s)
{
    return mini_stl::chrono::seconds(s);
}

constexpr mini_stl::chrono::duration<long double> operator ""s(long double s)
{
    return mini_stl::chrono::duration<long double>(s);
}

constexpr mini_stl::chrono::milliseconds operator ""ms(unsigned long long ms)
{
    return mini_stl::chrono::milliseconds(ms);
}

constexpr mini_stl::chrono::duration<long double, std::milli> operator ""ms(long double ms)
{
    return mini_stl::chrono::duration<long double, std::milli>(ms);
}

constexpr mini_stl::chrono::microseconds operator ""us(unsigned long long us)
{
    return mini_stl::chrono::microseconds(us);
}

constexpr mini_stl::chrono::duration<long double, std::micro> operator ""us(long double us)
{
    return mini_stl::chrono::duration<long double, std::micro>(us);
}

constexpr mini_stl::chrono::nanoseconds operator ""ns(unsigned long long ns)
{
    return mini_stl::chrono::nanoseconds(ns);
}

constexpr mini_stl::chrono::duration<long double, std::nano> operator ""ns(long double ns)
{
    return mini_stl::chrono::duration<long double, std::nano>(ns);
}

}   // namespace chrono_literals

}

#endif
