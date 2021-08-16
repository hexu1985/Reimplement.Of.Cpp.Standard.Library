#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;

#include "shared_ptr_operator_dereference2.hpp"

TEST(benchmark, shared_ptr_operator_dereference2)
{
    shared_ptr_operator_dereference2();
}

