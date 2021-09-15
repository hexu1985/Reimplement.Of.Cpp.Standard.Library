namespace {

void shared_ptr_operator_dereference() 
{
    shared_ptr<int> foo (new int);
    shared_ptr<int> bar (new int (100));

    *foo = *bar * 2;

    EXPECT_EQ(200, *foo);
    EXPECT_EQ(100, *bar);
}

}   // namespace
