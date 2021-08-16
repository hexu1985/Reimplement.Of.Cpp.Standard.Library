#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;
using std::make_shared;

#include "shared_ptr_relational_operators.hpp"

TEST(benchmark, shared_ptr_relational_operators)
{
    shared_ptr_relational_operators();
}

