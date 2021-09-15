namespace {

struct C : enable_shared_from_this<C> { };

void enable_shared_from_this_constructor2()
{
    shared_ptr<C> foo, bar;

    foo = make_shared<C>();

    bar = foo->shared_from_this();

    EXPECT_TRUE(!foo.owner_before(bar) && !bar.owner_before(foo));
}

}
