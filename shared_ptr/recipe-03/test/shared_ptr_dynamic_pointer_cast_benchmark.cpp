#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

#include "shared_ptr_dynamic_pointer_cast.hpp"

TEST(benchmark, shared_ptr_dynamic_pointer_cast)
{
    shared_ptr_dynamic_pointer_cast();
}

