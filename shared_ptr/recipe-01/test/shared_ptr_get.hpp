namespace {

void shared_ptr_get() 
{
    int* p = new int (10);
    shared_ptr<int> a (p);

    EXPECT_EQ(p, a.get());
    EXPECT_EQ(10, *a.get());
    EXPECT_EQ(10, *a);
    EXPECT_EQ(10, *p);
}

}   // namespace

