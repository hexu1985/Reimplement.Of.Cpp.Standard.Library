/**
 * @file shared_mutex.hpp
 * @brief 读写锁
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-09-04
 *
 * @see UNIX Network Programming, Volume 2: Interprocess Communications, Second Edition, Chapter 8.4
 */
#ifndef MINI_STL_SHARED_MUTEX_INC
#define MINI_STL_SHARED_MUTEX_INC

#include <mutex>
#include <condition_variable>

namespace Hx {

/**
 * @brief shared_mutex 类是一个同步原语，可用于保护共享数据不被多个线程同时访问。
 *      与便于独占访问的其他互斥类型不同，shared_mutex 拥有二个访问级别：
 *      共享 - 多个线程能共享同一互斥的所有权。 
 *      独占性 - 仅一个线程能占有互斥。 
 */
class shared_mutex {
private:
    std::mutex rw_mutex;        // basic lock on this struct
    std::condition_variable rw_condreaders;     // for reader threads waiting 
    std::condition_variable rw_condwriters;     // for writer threads waiting
    int rw_nwaitreaders;        // the number reader threads waiting
    int rw_nwaitwriters;        // the number writer threads waiting 
    int rw_refcount;            // -1 if writer has the lock, else # readers holding the lock

public:
    /**
     * @brief 构造互斥。调用后互斥在未锁定状态。
     */
    shared_mutex();

    /**
     * @brief 复制构造函数被删除。
     */
	shared_mutex(const shared_mutex&) = delete;

    /**
     * @brief 赋值运算符被删除，不可复制。
     */
	shared_mutex& operator= (const shared_mutex&) = delete;

    /**
     * @brief 销毁互斥。
     *
     * @warning 若互斥为任何线程占有，或若任何线程在保有任何互斥的所有权时终止，则行为未定义。
     */
    ~shared_mutex();

    /**
     * @brief 锁定互斥。若另一线程已锁定互斥，则到 lock 的调用将阻塞执行，直至获得锁。
     *
     * @warning 若已以任何模式（共享或排他性）占有 mutex 的线程调用 lock ，则行为未定义。
     */
	void lock();

    /**
     * @brief 尝试锁定互斥。立即返回。
     *
     * @return 成功获得锁时返回 true ，否则返回 false 。
     *
     * @note 允许此函数虚假地失败而返回 false ，即使互斥当前未为任何其他线程所锁定。
     *
     * @warning 若已以任何模式（共享或排他性）占有 mutex 的线程调用 try_lock ，则行为未定义。 
     */
	bool try_lock();

    /**
     * @brief 解锁互斥。 
     *
     * @warning 互斥必须为当前执行线程所锁定，否则行为未定义。 
     */
	void unlock();

    /**
     * @brief 获得互斥的共享所有权。若另一线程以排他性所有权保有互斥，则到 lock_shared 的调用将阻塞执行，直到能取得共享所有权。 
     *
     * @warning 若已以任何模式（排他性或共享）占有 mutex 的线程调用 lock_shared ，则行为未定义。
     */
	void lock_shared();

    /**
     * @brief 尝试以共享模式锁定互斥。立即返回。
     *
     * @return 成功获得锁时返回 true ，否则返回 false 。 
     *
     * @note 允许此函数虚假地失败并返回 false ，即使互斥当前不为任何其他线程所排他性锁定。 
     *
     * @warning 若调用方线程已以任何模式占有互斥，则行为未定义。
     */
	bool try_lock_shared();

    /**
     * @brief 将互斥从调用方线程的共享所有权释放。 
     *
     * @warning 当前执行线程必须以共享模式锁定互斥，否则行为未定义。
     */
	void unlock_shared(); 
};

}	// namespace Hx

#endif
