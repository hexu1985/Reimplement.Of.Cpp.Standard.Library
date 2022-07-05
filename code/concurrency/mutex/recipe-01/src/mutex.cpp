#include "mutex.hpp"
#include <cassert>
#include <system_error>

namespace Hx {

mutex::mutex()
{
    int err = pthread_mutex_init(&mtx_, NULL);
    if (err != 0) {
        throw std::system_error(err, std::system_category(), __func__);
    }
}

mutex::~mutex() 
{
    int err = pthread_mutex_destroy(&mtx_);
    (void) err;
    assert(err == 0);
}

void mutex::lock()
{
    while (true) {
        int err = pthread_mutex_lock(&mtx_);
        if (err == 0) {
            break;
        } else if (err == EINTR) {
            continue;
        } else {
            throw std::system_error(err, std::system_category(), __func__);
        }
    }
}

bool mutex::try_lock()
{
    int err = pthread_mutex_trylock(&mtx_);
    if (err == 0) {
        return true;
    } else if (err == EBUSY) {
        return false;
    } else {
        throw std::system_error(err, std::system_category(), __func__);
    }
    return false;
}

void mutex::unlock()
{
    int err = pthread_mutex_unlock(&mtx_);
    if (err != 0) {
        throw std::system_error(err, std::system_category(), __func__);
    }
}

}   // namespace Hx
