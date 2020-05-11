#include <memory>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

template <typename SharedPtr_Int>
void test_operator_assign()
{
    SharedPtr_Int p1(new int(3));
    SharedPtr_Int p2(new int(4));

    EXPECT_EQ(1, p1.use_count());
    EXPECT_TRUE(p1.get() != nullptr);
    EXPECT_EQ(3, *p1);

    EXPECT_EQ(1, p2.use_count());
    EXPECT_TRUE(p2.get() != nullptr);
    EXPECT_EQ(4, *p2);

    EXPECT_TRUE(p1.get() != p2.get());
    EXPECT_TRUE(*p1 != *p2);

    p1 = p2;

    EXPECT_EQ(2, p1.use_count());
    EXPECT_TRUE(p1.get() != nullptr);
    EXPECT_EQ(4, *p1);

    EXPECT_EQ(2, p2.use_count());
    EXPECT_TRUE(p2.get() != nullptr);
    EXPECT_EQ(4, *p2);

    EXPECT_TRUE(p1.get() == p2.get());
    EXPECT_TRUE(*p1 == *p2);
}

TEST(std_shared_ptr, operator_assign)
{
    test_operator_assign<std::shared_ptr<int>>();
}

TEST(my_shared_ptr, operator_assign)
{
    test_operator_assign<mini_stl::shared_ptr<int>>();
}
