namespace {

void weak_ptr_swap()
{
    shared_ptr<int> sp1 (new int(10));
    shared_ptr<int> sp2 (new int(20));

    weak_ptr<int> wp1(sp1);
    weak_ptr<int> wp2(sp2);

    wp1.swap(wp2);

    EXPECT_TRUE(sp1 == wp2.lock());
    EXPECT_TRUE(sp2 == wp1.lock());
}

}

