// -*- C++ -*-
// HeXu's
// 2013 May

#include "system_error.hpp"

namespace Hx {

const char* system_error::what() const throw()
{
    if (what_.empty()) {
        try 
        {
            what_ = this->std::runtime_error::what();
            if (!what_.empty())
                what_ += ": ";
            what_ += code_.message();
        }
        catch (...) 
        {
            return std::runtime_error::what(); 
        }
    }
    return what_.c_str();
}

}    // namespace Hx

