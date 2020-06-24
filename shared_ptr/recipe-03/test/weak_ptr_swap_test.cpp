#include <iostream>
#include <gtest/gtest.h>

#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using mini_stl::shared_ptr;
using mini_stl::weak_ptr;
using mini_stl::make_shared;

#include "weak_ptr_swap.hpp"

TEST(test, weak_ptr_swap)
{
    weak_ptr_swap();
}

