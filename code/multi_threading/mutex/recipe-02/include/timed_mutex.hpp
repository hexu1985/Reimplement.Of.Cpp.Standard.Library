// -*- C++ -*-
// HeXu's
// 2013 May

#ifndef MINI_STL_TIMED_MUTEX_INC
#define MINI_STL_TIMED_MUTEX_INC

#include <pthread.h>
#include <chrono>

namespace mini_stl {

/**
 * Timed mutex class
 * A timed mutex is a time lockable object that is designed to signal when 
 * critical sections of code need exclusive access, just like a regular mutex, 
 * but additionally supporting timed try-lock requests.
 */
class timed_mutex {
    pthread_mutex_t mtx_;        // native mutex handle

public:
    typedef pthread_mutex_t* native_handle_type;

    /**
     * Constructs the mutex. 
     * The mutex is in unlocked state after the call. 
     */
    timed_mutex();

    ~timed_mutex();

    timed_mutex(const timed_mutex&) = delete;
    timed_mutex& operator=(const timed_mutex&) = delete;

    /**
     * Locks the mutex. 
     * If another thread has already locked the mutex, a call to lock will 
     * block execution until the lock is acquired. If lock is called by a 
     * thread that already owns the mutex, the program may deadlock. 
     */
    void lock(); 
    
    /**
     * Tries to lock the mutex. Returns immediately. 
     * On successful lock acquisition returns true, otherwise returns false.
     * The behavior is undefined if the mutex is not unlocked 
     * before being destroyed, i.e. some thread still owns it. 
     */
    bool try_lock(); 

    /**
     * Unlocks the mutex.
     * The mutex must be unlocked by all threads that have successfully locked 
     * it before being destroyed. Otherwise, the behavior is undefined. 
     */
    void unlock(); 

    /**
     * Tries to lock the mutex. Blocks until specified rel_time 
     * has elapsed or the lock is acquired, whichever comes first. 
     * On successful lock acquisition returns true, otherwise returns false.
     */
    template <typename Rep, typename Period>
    bool try_lock_for(const std::chrono::duration<Rep, Period> &rel_time)
    {
        using namespace std::chrono;
        auto abs_time = system_clock::now()+rel_time;
        // convert abs_time to timespec type
        seconds sec = duration_cast<seconds>(abs_time.time_since_epoch());
        nanoseconds nsec = duration_cast<nanoseconds>((abs_time-sec).time_since_epoch());
        struct timespec ts;
        ts.tv_sec = sec.count();
        ts.tv_nsec = nsec.count();
        return timed_lock(&mtx_, &ts);
    }

    /**
     * Tries to lock the mutex. Blocks until specified abs_time 
     * has been reached or the lock is acquired, whichever comes first. 
     * On successful lock acquisition returns true, otherwise returns false. 
     */
    template <typename Clock, typename Duration>
    bool try_lock_until(const std::chrono::time_point<Clock, Duration> &abs_time)
    {
        return try_lock_for(abs_time-Clock::now());
    }

    /** 
     * Returns the underlying implementation-defined native handle object. 
     */
    native_handle_type native_handle() { return &mtx_; }

private:
    static bool timed_lock(pthread_mutex_t* mutex, const struct timespec* abstime);
};

}    // namespace mini_stl

#endif

