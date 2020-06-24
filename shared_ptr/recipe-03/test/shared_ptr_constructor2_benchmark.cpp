#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;

#include "shared_ptr_constructor2.hpp"

TEST(benchmark, shared_ptr_constructor2)
{
    shared_ptr_constructor2();
}

