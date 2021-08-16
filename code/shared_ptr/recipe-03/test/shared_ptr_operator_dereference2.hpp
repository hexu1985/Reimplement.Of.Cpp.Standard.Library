namespace {

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
