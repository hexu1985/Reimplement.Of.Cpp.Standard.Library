namespace {

struct Foo : public enable_shared_from_this<Foo> {
    Foo() {}  // 隐式调用 enable_shared_from_this 构造函数
    shared_ptr<Foo> getFoo() { return shared_from_this(); }
};
 
void enable_shared_from_this_constructor()
{
    shared_ptr<Foo> pf1(new Foo);
    auto pf2 = pf1->getFoo();  // 与 pf1 共享对象所有权

    EXPECT_EQ(2, pf1.use_count());
    EXPECT_EQ(2, pf2.use_count());
}

struct C : enable_shared_from_this<C> { };


void enable_shared_from_this_constructor2()
{
    shared_ptr<C> foo, bar;

    foo = make_shared<C>();

    bar = foo->shared_from_this();

    EXPECT_TRUE(!foo.owner_before(bar) && !bar.owner_before(foo));
}

}
