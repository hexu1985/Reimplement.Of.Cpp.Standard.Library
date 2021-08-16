namespace {

struct Foo : public enable_shared_from_this<Foo> {
    Foo() { std::cout << "Foo::Foo\n"; }
    ~Foo() { std::cout << "Foo::~Foo\n"; } 
    shared_ptr<Foo> getFoo() { return shared_from_this(); }
};

void enable_shared_from_this_shared()
{
    Foo *f = new Foo;
    shared_ptr<Foo> pf1;

    EXPECT_EQ(0, pf1.use_count());

    {
        shared_ptr<Foo> pf2(f);
        pf1 = pf2->getFoo();  // 与 pf2 的对象共享所有权
        EXPECT_EQ(2, pf1.use_count());
    }

    EXPECT_EQ(1, pf1.use_count());
}

}
