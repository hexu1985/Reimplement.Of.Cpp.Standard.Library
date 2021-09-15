namespace {

void shared_ptr_make_shared () {

    shared_ptr<int> foo = make_shared<int> (10);
    // same as:
    shared_ptr<int> foo2 (new int(10));

    auto bar = make_shared<int> (20);

    auto baz = make_shared<std::pair<int,int>> (30,40);

    EXPECT_EQ(10, *foo);
    EXPECT_EQ(10, *foo2);
    EXPECT_EQ(20, *bar);
    EXPECT_EQ(30, baz->first);
    EXPECT_EQ(40, baz->second);
}

}   // namespace
