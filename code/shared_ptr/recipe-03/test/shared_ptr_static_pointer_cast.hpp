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

void shared_ptr_static_pointer_cast()
{
    shared_ptr<A> foo;
    shared_ptr<B> bar;

    foo = make_shared<A>();

    // cast of potentially incomplete object, but ok as a static cast:
    bar = static_pointer_cast<B>(foo);

    ASSERT_STREQ("class A", foo->static_type);
    ASSERT_STREQ("class A", foo->dynamic_type);
    ASSERT_STREQ("class B", bar->static_type);
    ASSERT_STREQ("class A", bar->dynamic_type);
}

} 
