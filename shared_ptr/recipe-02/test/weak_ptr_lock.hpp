namespace {

void weak_ptr_lock()
{
    shared_ptr<int> sp1,sp2;
    weak_ptr<int> wp;
                                        // sharing group:
                                        // --------------
    sp1 = make_shared<int> (20);        // sp1
    wp = sp1;                           // sp1, wp

    sp2 = wp.lock();                    // sp1, wp, sp2
    sp1.reset();                        //      wp, sp2

    sp1 = wp.lock();                    // sp1, wp, sp2

    EXPECT_EQ(20, *sp1);
    EXPECT_EQ(20, *sp2);
}

}
