namespace {

weak_ptr<int> gw;

void weak_ptr_expired()
{
    {
        auto sp = make_shared<int>(42);
        gw = sp;

        EXPECT_FALSE(gw.expired());
    }

    EXPECT_TRUE(gw.expired());
}

}
