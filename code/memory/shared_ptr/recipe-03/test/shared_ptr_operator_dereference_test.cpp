#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

using Hx::shared_ptr;

#include "shared_ptr_operator_dereference.hpp"

TEST(test, shared_ptr_operator_dereference)
{
    shared_ptr_operator_dereference();
}

