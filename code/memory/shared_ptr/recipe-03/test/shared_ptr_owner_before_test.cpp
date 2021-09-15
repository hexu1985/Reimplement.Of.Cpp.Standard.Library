#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using mini_stl::shared_ptr;
using mini_stl::make_shared;
using mini_stl::weak_ptr;

#include "shared_ptr_owner_before.hpp"

TEST(test, shared_ptr_owner_before)
{
    shared_ptr_owner_before();
}

