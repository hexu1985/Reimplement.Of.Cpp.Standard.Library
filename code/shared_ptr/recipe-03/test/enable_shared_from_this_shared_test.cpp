#include <iostream>
#include <gtest/gtest.h>

#include "shared_ptr.hpp"
#include "weak_ptr.hpp"
#include "enable_shared_from_this.hpp"

using mini_stl::shared_ptr;
using mini_stl::weak_ptr;
using mini_stl::make_shared;
using mini_stl::enable_shared_from_this;

#include "enable_shared_from_this_shared.hpp"

TEST(test, enable_shared_from_this_shared)
{
    enable_shared_from_this_shared();
}

