#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

#include "shared_ptr_owner_before.hpp"

TEST(benchmark, shared_ptr_owner_before)
{
    shared_ptr_owner_before();
}

