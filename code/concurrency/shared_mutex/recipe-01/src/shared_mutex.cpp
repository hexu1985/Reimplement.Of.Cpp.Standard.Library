#include "shared_mutex.hpp"
#include <cassert>

namespace Hx {

shared_mutex::shared_mutex():
    rw_nwaitreaders(0),
    rw_nwaitwriters(0),
    rw_refcount(0)
{
}

shared_mutex::~shared_mutex()
{
    assert(rw_nwaitreaders == 0);
    assert(rw_nwaitwriters == 0);
    assert(rw_refcount == 0);
}

void shared_mutex::lock()
{
    std::unique_lock<std::mutex> lock(rw_mutex);
    while (rw_refcount != 0) {
        rw_nwaitwriters++;
        rw_condwriters.wait(lock);
        rw_nwaitwriters--;
    }
    rw_refcount = -1;
}

bool shared_mutex::try_lock()
{
    std::lock_guard<std::mutex> lock(rw_mutex);
    if (rw_refcount != 0) {
        return false;	    // held by either writer or reader(s)
    } else {
        rw_refcount = -1;	// available, indicate a writer has it
        return true;
    }
}

void shared_mutex::unlock()
{
    std::unique_lock<std::mutex> lock(rw_mutex);
    if (rw_refcount > 0)
        rw_refcount--;		// release a reader
    else if (rw_refcount == -1)
        rw_refcount = 0;	// release a writer

    // give preference to waiting writers over waiting readers
    if (rw_nwaitwriters > 0) {
        if (rw_refcount == 0)
            rw_condwriters.notify_one();
    } else if (rw_nwaitreaders > 0) {
        rw_condreaders.notify_all();
    }
}

void shared_mutex::lock_shared()
{
    std::unique_lock<std::mutex> lock(rw_mutex);
    // give preference to waiting writers
    while (rw_refcount < 0 || rw_nwaitwriters > 0) {
        rw_nwaitreaders++;
        rw_condreaders.wait(lock);
        rw_nwaitreaders--;
    }
    rw_refcount++;  // another reader has a read lock
}

bool shared_mutex::try_lock_shared()
{
    std::lock_guard<std::mutex> lock(rw_mutex);
    if (rw_refcount < 0 || rw_nwaitwriters > 0) {
        return false;	// held by a writer or waiting writers
    } else {
        rw_refcount++;	// increment count of reader locks
        return true;
    }
}

void shared_mutex::unlock_shared()
{
    unlock();
}

}   // Hx

