namespace {

struct A {
    static const char* static_type;
    const char* dynamic_type;
    A() { dynamic_type = static_type; }
};
struct B: A {
    static const char* static_type;
    B() { dynamic_type = static_type; }
};

const char* A::static_type = "class A";
const char* B::static_type = "class B";

void shared_ptr_dynamic_pointer_cast()
{
    shared_ptr<A> foo;
    shared_ptr<B> bar;

    bar = make_shared<B>();

    foo = dynamic_pointer_cast<A>(bar);

    ASSERT_STREQ("class A", foo->static_type);
    ASSERT_STREQ("class B", foo->dynamic_type);
    ASSERT_STREQ("class B", bar->static_type);
    ASSERT_STREQ("class B", bar->dynamic_type);
}

}
