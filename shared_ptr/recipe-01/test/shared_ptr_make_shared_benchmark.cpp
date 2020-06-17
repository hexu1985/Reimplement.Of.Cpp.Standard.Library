#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;
using std::make_shared;

#include "shared_ptr_make_shared.hpp"

TEST(benchmark, shared_ptr_make_shared)
{
    shared_ptr_make_shared();
}

