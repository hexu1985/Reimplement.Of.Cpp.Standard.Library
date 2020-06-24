namespace {

void shared_ptr_const_pointer_cast()
{
    shared_ptr<int> foo;
    shared_ptr<const int> bar;

    foo = make_shared<int>(10);

    bar = const_pointer_cast<const int>(foo);

    EXPECT_EQ(10, *bar);
    *foo = 20;
    EXPECT_EQ(20, *bar);
}

}

