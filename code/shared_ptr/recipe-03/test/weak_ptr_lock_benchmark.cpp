#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

#include "weak_ptr_lock.hpp"

TEST(benchmark, weak_ptr_lock)
{
    weak_ptr_lock();
}

