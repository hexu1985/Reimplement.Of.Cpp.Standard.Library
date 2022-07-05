#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

using Hx::shared_ptr;
using Hx::make_shared;

#include "shared_ptr_relational_operators.hpp"

TEST(test, shared_ptr_relational_operators)
{
    shared_ptr_relational_operators();
}

