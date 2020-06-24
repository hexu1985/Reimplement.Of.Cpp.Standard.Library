namespace {

void weak_ptr_reset()
{
    shared_ptr<int> sp (new int(10));

    weak_ptr<int> wp(sp);

    EXPECT_FALSE(wp.expired());

    wp.reset();

    EXPECT_TRUE(wp.expired());
}

}

