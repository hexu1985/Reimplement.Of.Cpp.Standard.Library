#pragma once

namespace mini_stl {

template <typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant; // using injected-class-name
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; } // since c++14
};

//typedef integral_constant<bool,true> true_type;
using true_type = integral_constant<bool,true>;

//typedef integral_constant<bool,false> false_type;
using false_type = integral_constant<bool,false>;


}   // namespace mini_stl
