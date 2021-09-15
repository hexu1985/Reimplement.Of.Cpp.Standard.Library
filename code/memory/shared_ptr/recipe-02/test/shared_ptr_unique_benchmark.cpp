#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;

#include "shared_ptr_unique.hpp"

TEST(benchmark, shared_ptr_unique)
{
    shared_ptr_unique();
}

