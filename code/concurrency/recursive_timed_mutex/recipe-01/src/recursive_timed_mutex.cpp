#include "recursive_timed_mutex.hpp"
#include <cassert>
#include <system_error>

namespace Hx {

recursive_timed_mutex::recursive_timed_mutex()
{
    // mutex attr recursive
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int err = pthread_mutex_init(&mtx_, &attr);
    pthread_mutexattr_destroy(&attr);
    if (err != 0) {
        throw std::system_error(err, std::system_category(), __func__);
    }
}

recursive_timed_mutex::~recursive_timed_mutex() 
{
    int err = pthread_mutex_destroy(&mtx_);
    (void) err;
    assert(err == 0);
}

void recursive_timed_mutex::lock()
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

bool recursive_timed_mutex::try_lock()
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

void recursive_timed_mutex::unlock()
{
    int err = pthread_mutex_unlock(&mtx_);
    if (err != 0) {
        throw std::system_error(err, std::system_category(), __func__);
    }
}

bool recursive_timed_mutex::timed_lock(pthread_mutex_t* mutex, const struct timespec* abstime)
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

}   // namespace Hx
