#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

using Hx::shared_ptr;
using Hx::make_shared;

#include "shared_ptr_make_shared.hpp"

TEST(test, shared_ptr_make_shared)
{
    shared_ptr_make_shared();
}

