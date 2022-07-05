#include <iostream>
#include <gtest/gtest.h>

#include "shared_ptr.hpp"
#include "weak_ptr.hpp"
#include "enable_shared_from_this.hpp"

using Hx::shared_ptr;
using Hx::weak_ptr;
using Hx::make_shared;
using Hx::enable_shared_from_this;

#include "enable_shared_from_this_operator_assign.hpp"

TEST(test, enable_shared_from_this_operator_assign)
{
    enable_shared_from_this_operator_assign();
}

