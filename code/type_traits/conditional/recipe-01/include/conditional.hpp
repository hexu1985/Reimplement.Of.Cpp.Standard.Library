#pragma once

namespace Hx {

template <bool B, typename T, typename F>
struct conditional { using type = T; };
 
template <typename T, typename F>
struct conditional<false, T, F> { using type = F; };

template < bool B, typename T, typename F >
using conditional_t = typename conditional<B,T,F>::type;

}   // namespace Hx

