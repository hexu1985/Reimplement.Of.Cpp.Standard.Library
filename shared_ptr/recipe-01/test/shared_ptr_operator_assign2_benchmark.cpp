#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;

#include "shared_ptr_operator_assign2.hpp"

TEST(benchmark, shared_ptr_operator_assign2)
{
    shared_ptr_operator_assign2();
}

