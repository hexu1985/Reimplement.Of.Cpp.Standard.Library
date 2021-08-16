#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
using std::enable_shared_from_this;

#include "enable_shared_from_this_shared.hpp"

TEST(benchmark, enable_shared_from_this_shared)
{
    enable_shared_from_this_shared();
}

