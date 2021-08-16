namespace {

void shared_ptr_operator_assign()
{
    shared_ptr<int> p1(new int(3));
    shared_ptr<int> p2(new int(4));

    EXPECT_EQ(1, p1.use_count());
    EXPECT_TRUE(p1.get() != nullptr);
    EXPECT_EQ(3, *p1);

    EXPECT_EQ(1, p2.use_count());
    EXPECT_TRUE(p2.get() != nullptr);
    EXPECT_EQ(4, *p2);

    EXPECT_TRUE(p1.get() != p2.get());
    EXPECT_TRUE(*p1 != *p2);

    p1 = p1;
    p2 = std::move(p2);

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

}   // namespace

