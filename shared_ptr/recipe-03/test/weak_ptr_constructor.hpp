namespace {

struct Foo {};

void weak_ptr_constructor()
{
    weak_ptr<Foo> w_ptr;

    {
        auto ptr = make_shared<Foo>();
        w_ptr = ptr;
        EXPECT_EQ(1, w_ptr.use_count());
    }

    EXPECT_EQ(0, w_ptr.use_count());
    EXPECT_TRUE(w_ptr.expired());
}

struct C {int* data;};

void weak_ptr_constructor2()
{
    shared_ptr<int> sp (new int);

    weak_ptr<int> wp1;
    weak_ptr<int> wp2 (wp1);
    weak_ptr<int> wp3 (sp);
    weak_ptr<int> wp4 (std::move(wp3));

    EXPECT_EQ(0, wp1.use_count());
    EXPECT_EQ(0, wp2.use_count());
    EXPECT_EQ(0, wp3.use_count());
    EXPECT_EQ(1, wp4.use_count());
}

}
