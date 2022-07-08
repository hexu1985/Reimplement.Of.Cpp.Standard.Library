#pragma once

#include <type_traits>
#include <tuple>
#include <utility>

namespace Hx {

template<class... Types>
std::tuple<Types&&...> forward_as_tuple (Types&&... args) noexcept
{
  return std::tuple<Types&&...>(std::forward<Types>(args)...);
}

}   // namespace Hx

