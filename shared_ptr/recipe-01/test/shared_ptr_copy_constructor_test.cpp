#include <iostream>
#include <stdexcept>
#include <memory>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

template <typename SharedPtr_Int>
void test_copy_constructor()
{
    SharedPtr_Int p1(new int(3));

    EXPECT_EQ(1, p1.use_count());
    EXPECT_TRUE(p1.get() != nullptr);
    EXPECT_EQ(3, *p1);

    SharedPtr_Int p2(p1);

    EXPECT_EQ(2, p1.use_count());
    EXPECT_TRUE(p1.get() != nullptr);
    EXPECT_EQ(3, *p1);

    EXPECT_EQ(2, p2.use_count());
    EXPECT_TRUE(p2.get() != nullptr);
    EXPECT_EQ(3, *p2);

    EXPECT_TRUE(p1.use_count() == p2.use_count());
    EXPECT_TRUE(p1.get() == p2.get());
    EXPECT_TRUE(*p1 == *p2);

    p1.reset();

    EXPECT_EQ(0, p1.use_count());
    EXPECT_TRUE(p1.get() == nullptr);

    EXPECT_EQ(1, p2.use_count());
    EXPECT_TRUE(p2.get() != nullptr);
    EXPECT_EQ(3, *p2);

    p2.reset();

    EXPECT_EQ(0, p1.use_count());
    EXPECT_TRUE(p1.get() == nullptr);

    EXPECT_EQ(0, p2.use_count());
    EXPECT_TRUE(p2.get() == nullptr);
}

TEST(std_shared_ptr, copy_constructor)
{
    test_copy_constructor<std::shared_ptr<int>>();
}

TEST(my_shared_ptr, copy_constructor)
{
    test_copy_constructor<mini_stl::shared_ptr<int>>();
}
