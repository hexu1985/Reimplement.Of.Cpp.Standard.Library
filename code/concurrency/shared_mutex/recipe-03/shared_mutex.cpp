#include "shared_mutex.hpp"
#include <cassert>

namespace Hx {

shared_mutex::shared_mutex():
    r_active(0), w_active(0), r_wait(0), w_wait(0)
{
}

shared_mutex::~shared_mutex()
{
    assert(r_active == 0);
    assert(w_active == 0);
    assert(r_wait == 0);
    assert(w_wait == 0);
}

void shared_mutex::lock()
{
    std::unique_lock<std::mutex> lock(mutex);
    if (w_active || r_active > 0) {
        w_wait++;
        while (w_active || r_active > 0) {
            write.wait(lock);
        }
        w_wait--;
    }
    w_active = 1;
}

bool shared_mutex::try_lock()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (w_active || r_active > 0) {
        return false;
    } else {
        w_active = 1;
        return true;
    }
}

void shared_mutex::unlock()
{
    std::unique_lock<std::mutex> lock(mutex);
    w_active = 0;
    if (w_wait > 0) {
        write.notify_one();
    } else if (r_wait > 0) {
        read.notify_all();
    } 
}

void shared_mutex::lock_shared()
{
    std::unique_lock<std::mutex> lock(mutex);
    if (w_active || w_wait > 0) {
        r_wait++;
        while (w_active || w_wait > 0) {
            read.wait(lock);
        }
        r_wait--;
    }
    r_active++;
}

bool shared_mutex::try_lock_shared()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (w_active || w_wait > 0) {
        return false;
    } else {
        r_active++;
        return true;
    }
}

void shared_mutex::unlock_shared()
{
    std::lock_guard<std::mutex> lock(mutex);
    r_active--;
    if (r_active == 0 && w_wait > 0) {
        write.notify_one();
    }
}

}   // Hx

