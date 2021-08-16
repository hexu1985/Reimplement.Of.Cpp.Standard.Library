#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;

#include "shared_ptr_operator_bool.hpp"

TEST(benchmark, shared_ptr_operator_bool)
{
    shared_ptr_operator_bool();
}

