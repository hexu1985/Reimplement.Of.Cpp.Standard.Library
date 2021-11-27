#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <thread>
#include <boost/thread/shared_mutex.hpp>

boost::shared_mutex rwlock;

void thread1();
void thread2();

inline
std::ostream& operator<<(std::ostream &os,
        const std::chrono::time_point<std::chrono::system_clock> &t)
{
    const auto tt (std::chrono::system_clock::to_time_t(t));
    const auto loct (std::localtime(&tt));
    return os << std::put_time(loct, "%c");
}

inline
std::chrono::time_point<std::chrono::system_clock> gf_time()
{
    return std::chrono::system_clock::now();
}

inline
void sleep(int nsecs)
{
    std::this_thread::sleep_for(std::chrono::seconds(nsecs));
}

int main(int argc, char *argv[])
{
    std::thread thr1, thr2;

    rwlock.lock_shared();   /* parent read locks entire file */
    std::cout << gf_time() << ": parent has read lock" << std::endl;

    thr1 = std::thread(&thread1);
    thr2 = std::thread(&thread2);

	/* 4parent */
    sleep(5);
    rwlock.unlock_shared();
    std::cout << gf_time() << ": parent releases read lock" << std::endl;

    thr1.join();
    thr2.join();

    return 0;
}

void thread1()
{
    sleep(1);
    std::cout << gf_time() << ": first child tries to obtain write lock" << std::endl;
    rwlock.lock();  /* this should block */
    std::cout << gf_time() << ": first child obtains write lock" << std::endl;
    sleep(2);
    rwlock.unlock();
    std::cout << gf_time() << ": first child releases write lock" << std::endl;
}

void thread2()
{
    /* 4second child */
    sleep(3);
    std::cout << gf_time() << ": second child tries to obtain read lock" << std::endl;
    rwlock.lock_shared();
    std::cout << gf_time() << ": second child obtains read lock" << std::endl;
    sleep(4);
    rwlock.unlock_shared();
    std::cout << gf_time() << ": second child releases read lock" << std::endl;
}
