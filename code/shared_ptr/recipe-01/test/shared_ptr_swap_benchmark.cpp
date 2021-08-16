#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;

#include "shared_ptr_swap.hpp"

TEST(benchmark, shared_ptr_swap)
{
    shared_ptr_swap();
}

