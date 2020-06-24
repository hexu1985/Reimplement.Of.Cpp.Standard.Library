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

}
