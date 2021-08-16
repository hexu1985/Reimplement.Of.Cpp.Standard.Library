namespace {

void shared_ptr_swap() 
{
    shared_ptr<int> foo (new int(10));
    shared_ptr<int> bar (new int(20));

    EXPECT_EQ(10, *foo);
    EXPECT_EQ(20, *bar);

    foo.swap(bar);

    EXPECT_EQ(20, *foo);
    EXPECT_EQ(10, *bar);
}

}   // namespace
