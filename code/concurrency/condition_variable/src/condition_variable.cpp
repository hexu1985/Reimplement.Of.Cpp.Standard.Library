#include "condition_variable.hpp"
#include <pthread.h>
#include <cassert>
#include <chrono>
#include <mutex>
#include <type_traits>
#include <system_error>

namespace Hx {

condition_variable::condition_variable()
{
    int err = pthread_cond_init(&cnd_, NULL);
    if (err != 0) {
        throw std::system_error(err, std::system_category(), __func__);
    }
}

condition_variable::~condition_variable()
{
    int err = pthread_cond_destroy(&cnd_); 
    (void) err;
    assert(err == 0);
}

void condition_variable::notify_one() noexcept 
{
    int err = pthread_cond_signal(&cnd_); 
    (void) err;
    assert(err == 0);
}

void condition_variable::notify_all() noexcept 
{
    int err =pthread_cond_broadcast(&cnd_); 
    (void) err;
    assert(err == 0);
}

void condition_variable::wait(pthread_cond_t* cond, pthread_mutex_t* mutex)
{
    while (true) {
        int err = pthread_cond_wait(cond, mutex);
        if (err == 0) {
            break;
        } else if (err == EINTR) {
            continue;
        } else {
            throw std::system_error(err, std::system_category(), __func__);
        }
    }
}

cv_status condition_variable::timed_wait(pthread_cond_t* cond, pthread_mutex_t* mutex, 
    const struct timespec* abstime)
{
    while (true) {
        int err = pthread_cond_timedwait(cond, mutex, abstime);
        if (err == 0) {
            return cv_status::no_timeout;
        } else if (err == ETIMEDOUT) {
            return cv_status::timeout;
        } else if (err == EINTR) {
            continue;
        } else {
            throw std::system_error(err, std::system_category(), __func__);
        }
    }
}

}   // namespace Hx
