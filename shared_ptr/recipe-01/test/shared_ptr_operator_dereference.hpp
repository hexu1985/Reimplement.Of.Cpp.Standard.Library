namespace {

void shared_ptr_operator_dereference() 
{
    shared_ptr<int> foo (new int);
    shared_ptr<int> bar (new int (100));

    *foo = *bar * 2;

    EXPECT_EQ(200, *foo);
    EXPECT_EQ(100, *bar);
}

struct C { int a; int b; };

void shared_ptr_operator_dereference2() 
{
    shared_ptr<C> foo;
    shared_ptr<C> bar (new C);

    foo = bar;

    foo->a = 10;
    bar->b = 20;

    EXPECT_EQ(10, bar->a);
    EXPECT_EQ(20, foo->b);
}

}   // namespace
