namespace {

struct deleter
{
    void operator ()(int *p)
    {
        std::cout << "destroying int at "
            << p << '\n';
        delete p;
    }
};

void shared_ptr_constructor()
{
    shared_ptr<int> p1;
    shared_ptr<int> p2(new int(3));
    shared_ptr<int> p3(nullptr);
    shared_ptr<int> p4(new int(3), deleter());
    shared_ptr<int> p5(p4);
    shared_ptr<int> p6(std::move(p5));

    EXPECT_EQ(0, p1.use_count());
    EXPECT_EQ(1, p2.use_count());
    EXPECT_EQ(0, p3.use_count());
    EXPECT_EQ(2, p4.use_count());
    EXPECT_EQ(0, p5.use_count());
    EXPECT_EQ(2, p6.use_count());
}

}   // namespace
