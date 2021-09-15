#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;

#include "shared_ptr_get.hpp"

TEST(benchmark, shared_ptr_get)
{
    shared_ptr_get();
}

