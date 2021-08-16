namespace {

void shared_ptr_relational_operators() 
{
	shared_ptr<int> a,b,c,d;

	a = make_shared<int> (10);
	b = make_shared<int> (10);
	c = b;

    EXPECT_EQ(false, (a==b));
    EXPECT_EQ(true, (b==c));
    EXPECT_EQ(false, (c==d));
    EXPECT_EQ(true, (a!=nullptr));
    EXPECT_EQ(true, (b!=nullptr)); 
    EXPECT_EQ(true, (c!=nullptr)); 
    EXPECT_EQ(false, (d!=nullptr));
}

}   // namespace
