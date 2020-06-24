#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;

#include "shared_ptr_copy_constructor.hpp"

TEST(benchmark, shared_ptr_copy_constructor)
{
    shared_ptr_copy_constructor();
}

