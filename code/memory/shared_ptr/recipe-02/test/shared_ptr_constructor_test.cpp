#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

using Hx::shared_ptr;

#include "shared_ptr_constructor.hpp"

TEST(test, shared_ptr_constructor)
{
    shared_ptr_constructor();
}

