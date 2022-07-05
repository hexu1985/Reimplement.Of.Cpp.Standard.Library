// -*- C++ -*-
// HeXu's
// 2013 Jun

#ifndef MINI_STL_RECURSIVE_TIMED_MUTEX_INC
#define MINI_STL_RECURSIVE_TIMED_MUTEX_INC

#include <pthread.h>
#include <chrono>

namespace Hx {

/**
 * The recursive_timed_mutex class is a synchronization primitive 
 * that can be used to protect shared data from 
 * being simultaneously accessed by multiple threads.
 * In a manner similar to recursive_mutex, recursive_timed_mutex 
 * provides exclusive, recursive ownership semantics. 
 * In addition, recursive_timed_mutex provides the ability to 
 * attempt to claim ownership of a recursive_timed_mutex 
 * with a timeout via the try_lock_for and try_lock_until methods.
 * The recursive_timed_mutex class is non-copyable. 
 */
class recursive_timed_mutex {
    pthread_mutex_t mtx_;        // native mutex handle

public:
    typedef pthread_mutex_t* native_handle_type;

	/**
	 * Constructs the mutex. 
	 * The mutex is in unlocked state after the call. 
	 */
	recursive_timed_mutex();

    /** Destroys the mutex. */
    ~recursive_timed_mutex();

    recursive_timed_mutex(const recursive_timed_mutex&) = delete;
    recursive_timed_mutex& operator=(const recursive_timed_mutex&) = delete;

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

}	// namespace Hx

#endif

