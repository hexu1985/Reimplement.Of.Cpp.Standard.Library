#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using Hx::shared_ptr;
using Hx::make_shared;
using Hx::weak_ptr;

#include "shared_ptr_owner_before.hpp"

TEST(test, shared_ptr_owner_before)
{
    shared_ptr_owner_before();
}

