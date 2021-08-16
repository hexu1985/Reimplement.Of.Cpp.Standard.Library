namespace {

class SharedInt : public enable_shared_from_this<SharedInt> {
public:
    explicit SharedInt(int n) : mNumber(n) {}
    SharedInt(const SharedInt&) = default;
    SharedInt(SharedInt&&) = default;
    ~SharedInt() = default;

    // 两个赋值运算符都使用 enable_shared_from_this::operator=
    SharedInt& operator=(const SharedInt&) = default;
    SharedInt& operator=(SharedInt&&) = default;

    int number() const { return mNumber; }

private:
    int mNumber;
};

void enable_shared_from_this_operator_assign() 
{
    shared_ptr<SharedInt> a = make_shared<SharedInt>(2);
    shared_ptr<SharedInt> b = make_shared<SharedInt>(4);
    *a = *b;

    EXPECT_EQ(4, a->number());
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a == a->shared_from_this());
    EXPECT_TRUE(b == b->shared_from_this());
}

}
