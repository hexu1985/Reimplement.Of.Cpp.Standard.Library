#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

using mini_stl::shared_ptr;
using mini_stl::make_shared;
using mini_stl::static_pointer_cast;

#include "shared_ptr_static_pointer_cast.hpp"

TEST(test, shared_ptr_static_pointer_cast)
{
    shared_ptr_static_pointer_cast();
}

