namespace {

struct C {int* data;};

void shared_ptr_constructor2()
{ 
    shared_ptr<int> p1;
    shared_ptr<int> p2 (nullptr);
    shared_ptr<int> p3 (new int);
    shared_ptr<int> p4 (new int, std::default_delete<int>());
    shared_ptr<int> p5 (p4);
    shared_ptr<int> p6 (std::move(p5));
    shared_ptr<C> obj (new C);
    shared_ptr<int> p7 (obj, obj->data);

    EXPECT_EQ(0, p1.use_count());
    EXPECT_EQ(0, p2.use_count());
    EXPECT_EQ(1, p3.use_count());
    EXPECT_EQ(2, p4.use_count());
    EXPECT_EQ(0, p5.use_count());
    EXPECT_EQ(2, p6.use_count());
    EXPECT_EQ(2, p7.use_count());
}

}   // namespace

