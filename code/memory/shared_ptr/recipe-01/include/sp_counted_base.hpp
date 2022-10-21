/**
 * @file sp_counted_base.hpp
 * @brief shared_ptr的引用计数基类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-04-03
 */
#ifndef MINI_STL_SP_COUNTED_BASE_INC
#define MINI_STL_SP_COUNTED_BASE_INC

#include <atomic>

namespace Hx {

/**
 * @brief shared_ptr的引用计数基类
 */
class sp_counted_base {
private:
    std::atomic<long> use_count_;   // #shared: 共享引用计数

    sp_counted_base(const sp_counted_base&) = delete;
    sp_counted_base& operator=(const sp_counted_base&) = delete;

public:
    /**
     * @brief 默认构造函数, 引用计数初始化为1
     */
    sp_counted_base();

    /**
     * @brief 虚析构函数
     */
    virtual ~sp_counted_base(); 

    /**
     * @brief 当共享引用计数(use_count_)递减至0, 释放*this所管理的共享对象
     */
    virtual void dispose() = 0;

    /**
     * @brief 当共享引用计数(use_count_)递减至0, 释放*this本身
     */
    virtual void destroy();

    /**
     * @brief 增加共享引用, 共享引用计数(use_count_)加1
     */
    void add_ref_copy();

    /**
     * @brief 释放共享引用, 将共享引用计数(use_count_)减1,
     *        如果共享引用计数(use_count_)减至0,
     *        释放*this管理的共享对象, 并释放*this本身.
     */
    void release();

    /**
     * @brief 获取*this管理的共享对象的指针
     *
     * @return *this管理的共享对象的指针
     */
    virtual void* get_pointer() = 0;

    /**
     * @brief 获取指向deleter的指针
     *
     * @return 指向deleter的指针
     */
    virtual void* get_deleter() = 0;

    /**
     * @brief 获取当前共享引用计数
     *
     * @return 当前共享引用计数值
     */
    long use_count() const;
};

struct sp_counted_base_tag {};

}   // namespace Hx

#endif
