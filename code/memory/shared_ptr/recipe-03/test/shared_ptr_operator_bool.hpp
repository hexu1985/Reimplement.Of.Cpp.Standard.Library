namespace {

void shared_ptr_operator_bool() 
{
	shared_ptr<int> foo;
	shared_ptr<int> bar (new int(34));

    EXPECT_FALSE(foo);
    EXPECT_TRUE(bar);
    EXPECT_EQ(34, *bar);
}

}   // namespace
