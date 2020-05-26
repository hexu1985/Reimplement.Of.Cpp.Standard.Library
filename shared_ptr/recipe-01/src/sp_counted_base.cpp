#include "sp_counted_base.hpp"

namespace mini_stl {

sp_counted_base::sp_counted_base(): use_count_(1) 
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

void sp_counted_base::release() 
{
    if(--use_count_ == 0) {
        dispose();
        destroy();
    }
}

long sp_counted_base::use_count() const
{
    return use_count_;
}

}   // namespace mini_stl
