#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;

#include "shared_ptr_operator_assign.hpp"

TEST(benchmark, shared_ptr_operator_assign)
{
    shared_ptr_operator_assign();
    shared_ptr_operator_assign2();
}

