#include "timed_mutex.hpp"
#include <cassert>
#include <system_error>

namespace mini_stl {

timed_mutex::timed_mutex()
{
    int err = pthread_mutex_init(&mtx_, NULL);
    if (err != 0) {
        throw std::system_error(err, std::system_category(), __func__);
    }
}

timed_mutex::~timed_mutex() 
{
    int err = pthread_mutex_destroy(&mtx_);
    (void) err;
    assert(err == 0);
}

void timed_mutex::lock()
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

bool timed_mutex::try_lock()
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

void timed_mutex::unlock()
{
    int err = pthread_mutex_unlock(&mtx_);
    if (err != 0) {
        throw std::system_error(err, std::system_category(), __func__);
    }
}

bool timed_mutex::timed_lock(pthread_mutex_t* mutex, const struct timespec* abstime)
{
    while (true) {
        int err = pthread_mutex_timedlock(mutex, abstime);
        if (err == 0) {
            return true;
        } else if (err == ETIMEDOUT) {
            return false;
        } else if (err == EINTR) {
            continue;
        } else {
            throw std::system_error(err, std::system_category(), __func__);
        }
    }
}

}   // namespace mini_stl
