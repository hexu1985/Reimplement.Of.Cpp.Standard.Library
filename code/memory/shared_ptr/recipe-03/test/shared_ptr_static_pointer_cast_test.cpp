#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

using Hx::shared_ptr;
using Hx::make_shared;
using Hx::static_pointer_cast;

#include "shared_ptr_static_pointer_cast.hpp"

TEST(test, shared_ptr_static_pointer_cast)
{
    shared_ptr_static_pointer_cast();
}

