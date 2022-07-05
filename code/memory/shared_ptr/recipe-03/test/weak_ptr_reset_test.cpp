#include <iostream>
#include <gtest/gtest.h>

#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

using Hx::shared_ptr;
using Hx::weak_ptr;
using Hx::make_shared;

#include "weak_ptr_reset.hpp"

TEST(test, weak_ptr_reset)
{
    weak_ptr_reset();
}

