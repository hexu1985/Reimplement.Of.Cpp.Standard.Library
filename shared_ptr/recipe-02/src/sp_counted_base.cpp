#include "sp_counted_base.hpp"

namespace mini_stl {

sp_counted_base::sp_counted_base(): use_count_(1), weak_count_(1)
{
}

sp_counted_base::~sp_counted_base()
{
}

void sp_counted_base::destroy()
{
    delete this;
}

void sp_counted_base::add_ref_copy()
{
    ++use_count_;
}

bool sp_counted_base::add_ref_lock()
{
    long r = use_count_;
    if (r != 0) ++use_count_;
    return r;
}

void sp_counted_base::release() 
{
    if(--use_count_ == 0) {
        dispose();
        weak_release();
    }
}

void sp_counted_base::weak_add_ref()
{
    ++weak_count_;
}

void sp_counted_base::weak_release()
{
    if (--weak_count_ == 0) {
        destroy();
    }
}

long sp_counted_base::use_count() const
{
    return use_count_;
}

}   // namespace mini_stl
