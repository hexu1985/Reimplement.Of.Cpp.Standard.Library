#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

using Hx::shared_ptr;

#include "shared_ptr_swap.hpp"

TEST(test, shared_ptr_swap)
{
    shared_ptr_swap();
}

