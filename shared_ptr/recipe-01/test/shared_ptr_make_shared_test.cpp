#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

using mini_stl::shared_ptr;
using mini_stl::make_shared;

#include "shared_ptr_make_shared.hpp"

TEST(test, shared_ptr_make_shared)
{
    shared_ptr_make_shared();
}

