// -*- C++ -*-
// HeXu's
// 2013 May

#ifndef MINI_STL_CONDITION_VARIABLE_INC
#define MINI_STL_CONDITION_VARIABLE_INC

#include <pthread.h>
#include <cassert>
#include <chrono>
#include <mutex>
#include <type_traits>
#include <system_error>

namespace Hx {

/**
 * The scoped enumeration cv_status describes whether a timed wait 
 * returned because of timeout or not. 
 */
enum class cv_status { 
    timeout, 
    no_timeout, 
};

/**
 * The condition_variable class is a synchronization primitive 
 * that can be used to block a thread, or multiple threads 
 * at the same time, until: a notification is received 
 * from another thread, a timeout expires, 
 * or a spurious wakeup occurs.
 * Any thread that intends to wait on condition_variable 
 * has to acquire a unique_lock first. 
 * The wait operations atomically release the mutex 
 * and suspend the execution of the thread. 
 * When the condition variable is notified, 
 * the thread is awakened, and the mutex is reacquired. 
 */
class condition_variable {
    pthread_cond_t cnd_;    // native condition variable

public:
    typedef    pthread_cond_t* native_handle_type;

    /** Constructs an object of type condition_variable. */
    condition_variable()
    {
        static_assert(std::is_same<std::mutex::native_handle_type, pthread_mutex_t*>::value);
        int err = pthread_cond_init(&cnd_, NULL);
        if (err != 0) {
            throw std::system_error(err, std::system_category(), __func__);
        }
    }

    condition_variable(const condition_variable&) = delete;
    condition_variable& operator=(const condition_variable&) = delete;

    /** Destroys the object of type condition_variable. */
    ~condition_variable()
    {
        int err = pthread_cond_destroy(&cnd_); 
        (void) err;
        assert(err == 0);
    }

    /**
     * Notify one
     * Unblocks one of the threads currently waiting for this condition.
     * If no threads are waiting, the function does nothing.
     * If more than one, it is unspecified which of the threads is selected.
     */
    void notify_one() noexcept 
    {
        int err = pthread_cond_signal(&cnd_); 
        (void) err;
        assert(err == 0);
    }

    /**
     * Notify all
     * Unblocks all threads currently waiting for this condition.
     * If no threads are waiting, the function does nothing.
     */
    void notify_all() noexcept 
    {
        int err =pthread_cond_broadcast(&cnd_); 
        (void) err;
        assert(err == 0);
    }

    /**
     * Wait until notified
     * The execution of the current thread 
     * (which shall have locked lck's mutex) is blocked until notified.
     */
    void wait(std::unique_lock<std::mutex>& lck)
    {
        while (true) {
            int err = pthread_cond_wait(&cnd_, lck.mutex()->native_handle());
            if (err == 0) {
                break;
            } else if (err == EINTR) {
                continue;
            } else {
                throw std::system_error(err, std::system_category(), __func__);
            }
        }
    }

    /**
     * If pred is specified, the function only blocks if pred returns false, 
     * and notifications can only unblock the thread when it becomes true 
     * (which is specially useful to check against spurious wake-up calls). 
     */
    template <typename Predicate>
    void wait(std::unique_lock<std::mutex>& lck, Predicate pred)
    {
        while (!pred()) {
            wait(lck);
        }
    }

    /**
     * Wait for timeout or until notified
     * The execution of the current thread (which shall have locked lck's mutex)
     * is blocked during rel_time, or until notified 
     * (if the latter happens first).
     */
    template <typename Rep, typename Period>
    cv_status wait_for(std::unique_lock<std::mutex>& lck, const std::chrono::duration<Rep, Period>& rel_time)
    {
        using namespace std::chrono;
        auto abs_time = system_clock::now()+rel_time;
        // convert abs_time to timespec type
        seconds sec = duration_cast<seconds>(abs_time.time_since_epoch());
        nanoseconds nsec = duration_cast<nanoseconds>((abs_time-sec).time_since_epoch());
        struct timespec ts;
        ts.tv_sec = sec.count();
        ts.tv_nsec = nsec.count();
        return timed_wait(&cnd_, lck.mutex()->native_handle(), &ts);
    }

    /**
     * If pred is specified, the function only blocks if pred returns false, 
     * and notifications can only unblock the thread when it becomes true 
     * (which is especially useful to check against spurious wake-up calls). 
     */
    template <typename Rep, typename Period, typename Predicate>
    bool wait_for(std::unique_lock<std::mutex>& lck, 
        const std::chrono::duration<Rep, Period>& rel_time, Predicate pred)
    {
        using namespace std::chrono;
        system_clock::time_point now = system_clock::now();
        return wait_until(lck, now+rel_time, pred);
    }

    /**
     * Wait until notified or time point
     * The execution of the current thread (which shall have locked lck's mutex)
     * is blocked either until notified or until abs_time, 
     * whichever happens first.
     */
    template <typename Clock, typename Duration>
    cv_status wait_until(std::unique_lock<std::mutex>& lck, 
        const std::chrono::time_point<Clock, Duration>& abs_time)
    {
        return wait_for(lck, abs_time-Clock::now());
    }

    /**
     * If pred is specified, the function only blocks if pred returns false, 
     * and notifications can only unblock the thread when it becomes true 
     * (which is especially useful to check against spurious wake-up calls). 
     */
    template <typename Clock, typename Duration, typename Predicate>
    bool wait_until(std::unique_lock<std::mutex>& lck, 
        const std::chrono::time_point<Clock, Duration>& abs_time, Predicate pred)
    {
        while (!pred()) {
            if (wait_until(lck, abs_time) == cv_status::timeout) {
                return pred();
            }
        }
        return true;
    }

    /** Accesses the native handle of *this. */
    native_handle_type native_handle() { return &cnd_; }

private:
    static
    cv_status timed_wait(pthread_cond_t* cond, pthread_mutex_t* mutex, 
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
};

}    // namespace Hx

#endif

