namespace {

void shared_ptr_unique() 
{
    shared_ptr<int> foo;
    shared_ptr<int> bar (new int);

    EXPECT_FALSE(foo.unique());

    foo = bar;

    EXPECT_FALSE(foo.unique());

    bar = nullptr;

    EXPECT_TRUE(foo.unique());
}

}   // namespace

/*
Output:
foo unique?
1: false
2: false
3: true
*/

