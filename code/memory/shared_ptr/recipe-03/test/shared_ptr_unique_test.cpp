#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

using mini_stl::shared_ptr;

#include "shared_ptr_unique.hpp"

TEST(test, shared_ptr_unique)
{
    shared_ptr_unique();
}

