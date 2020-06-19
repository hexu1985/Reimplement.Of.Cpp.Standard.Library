/**
 * @file sp_counted_impl.hpp
 * @brief shared_ptr的引用计数子类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-04-03
 */
#ifndef MINI_STL_SP_COUNTED_IMPL_INC
#define MINI_STL_SP_COUNTED_IMPL_INC

#include "sp_counted_base.hpp"

namespace mini_stl {

/**
 * @brief shared_ptr的引用计数子类: 内嵌共享对象, 为make_shared提供支持
 *
 * @tparam T
 */
template <typename T>
class sp_counted_impl: public sp_counted_base {
private:
    T data_;

    sp_counted_impl(const sp_counted_impl &) = delete;
    sp_counted_impl &operator =(const sp_counted_impl &) = delete;

public:
    sp_counted_impl(): data_() {}

    template <typename ...Args>
    sp_counted_impl(Args &&...args): data_(std::forward<Args>(args)...) {}

    // do nothing
    void dispose() override {}

    void *get_pointer() override { return &data_; }

    void *get_deleter() override { return nullptr; }
};

/**
 * @brief shared_ptr的引用计数子类: 使用默认的delete运算符释放共享对象的指针
 *
 * @tparam T 共享对象的类型
 */
template <typename T>
class sp_counted_impl_p: public sp_counted_base {
private:
    T *p_;  // pointer

    sp_counted_impl_p(const sp_counted_impl_p &) = delete;
    sp_counted_impl_p &operator =(const sp_counted_impl_p &) = delete;

public:
    /**
     * @brief 构造函数
     *
     * @param p 指向共享对象的指针
     */
    explicit sp_counted_impl_p(T *p): p_(p) {}

    /**
     * @brief 当共享引用计数(use_count_)递减至0, 释放*this所管理的共享对象
     *
     * @note 采用默认策略释放*this管理的共享对象: delete运算符
     */
    void dispose() override { delete p_; }

    /**
     * @brief 获取*this管理的共享对象的指针
     *
     * @return *this管理的共享对象的指针
     */
    void *get_pointer() override { return p_; }

    /**
     * @brief 获取指向deleter的指针
     *
     * @return nullptr
     */
    void *get_deleter() override { return nullptr; }
};

/**
 * @brief  shared_ptr的引用计数子类: 使用自定义的deleter
 *
 * @tparam P 共享对象的指针类型: 如果T为共享对象类型, P相当于T*
 * @tparam D 自定义的deleter的类型
 */
template <typename P, typename D>
class sp_counted_impl_pd: public sp_counted_base {
private:
    P p_;       // pointer
    D del_;     // deleter

    sp_counted_impl_pd(const sp_counted_impl_pd &) = delete;
    sp_counted_impl_pd &operator =(const sp_counted_impl_pd &) = delete;

public:
    /**
     * @brief 构造函数
     *
     * @param p 指向共享对象的指针
     * @param d deleter的引用
     */
    sp_counted_impl_pd(P p, D &d): p_(p), del_(d) {}

    /**
     * @brief 构造函数
     *
     * @param p 指向共享对象的指针
     */
    sp_counted_impl_pd(P p): p_(p), del_() {}

    /**
     * @brief 当共享引用计数(use_count_)递减至0, 释放*this所管理的共享对象
     *
     * @note 采用Deleter子对象释放*this管理的共享对象
     */
    void dispose() override { del_(p_); }

    /**
     * @brief 获取*this管理的共享对象的指针
     *
     * @return *this管理的共享对象的指针
     */
    void *get_pointer() override { return p_; }

    /**
     * @brief 获取指向deleter的指针
     *
     * @return 指向deleter的指针
     */
    void *get_deleter() override { return &del_; }
};

}   // namespace mini_stl

#endif
