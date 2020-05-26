#include <iostream>
#include <memory>
#include <gtest/gtest.h>
#include "shared_ptr.hpp"

struct deleter
{
    void operator ()(int *p)
    {
        std::cout << "destroying int at "
            << p << '\n';
        delete p;
    }
};

template <typename SharedPtr_Int>
void test_constructor()
{
    SharedPtr_Int p1;
    SharedPtr_Int p2(new int(3));
    SharedPtr_Int p3(nullptr);
    SharedPtr_Int p4(new int(3), deleter());

    EXPECT_EQ(0, p1.use_count());
    EXPECT_EQ(1, p2.use_count());
    EXPECT_EQ(0, p3.use_count());
    EXPECT_EQ(1, p4.use_count());
}

TEST(std_shared_ptr, constructor)
{
    test_constructor<std::shared_ptr<int>>();
}

TEST(my_shared_ptr, constructor)
{
    test_constructor<mini_stl::shared_ptr<int>>();
}
