namespace {

struct Foo {
    int n1;
    int n2; 
    Foo(int a, int b) : n1(a), n2(b) {}
};

void shared_ptr_owner_before()
{   
    auto p1 = make_shared<Foo>(1, 2);
    shared_ptr<int> p2(p1, &p1->n1);
    shared_ptr<int> p3(p1, &p1->n2);

    EXPECT_TRUE(p2 < p3);
    EXPECT_FALSE(p3 < p2);
    EXPECT_FALSE(p2.owner_before(p3));
    EXPECT_FALSE(p3.owner_before(p2));

    weak_ptr<int> w2(p2);
    weak_ptr<int> w3(p3);
    EXPECT_FALSE(w2.owner_before(w3));
    EXPECT_FALSE(w3.owner_before(w2));
}

} 

