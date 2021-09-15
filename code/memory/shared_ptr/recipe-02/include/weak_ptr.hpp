/**
 * @file weak_ptr.hpp
 * @brief 拥有共享对象所有权语义的智能指针, 弱引用指针
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-06-16
 */
#ifndef MINI_STL_WEAK_PTR_INC
#define MINI_STL_WEAK_PTR_INC

#include "sp_counted_base.hpp"

namespace mini_stl {

template <typename T>
class shared_ptr;

template <typename T>
class weak_ptr;

/**
 * @brief 拥有共享对象所有权语义的智能指针, 弱引用指针
 *
 * @tparam T 共享对象的类型
 */
template <typename T>
class weak_ptr {
public:
    typedef T element_type;

private:
    sp_counted_base* pi_ = nullptr;

    typedef weak_ptr<T> this_type;

    template<typename> friend class shared_ptr;
    template<typename> friend class weak_ptr;

public:
    /**
     * @brief 默认构造函数. 构造空weak_ptr 
     */
    weak_ptr() {}

    /**
     * @brief 构造新的weak_ptr, 它共享r所管理的对象.
     *        若r不管理对象, 则*this亦不管理对象.
     *
     * @param r 被共享的weak_ptr
     */
    weak_ptr(const weak_ptr& r): pi_(r.pi_)
    {
        if (pi_ != nullptr) pi_->weak_add_ref();
    }

    /**
     * @brief 移动构造函数. 
     *        从r移动weak_ptr实例到*this.
     *        之后, r为空且r.use_count()==0
     *
     * @param r 被移动的weak_ptr
     */
    weak_ptr(weak_ptr&& r) noexcept: pi_(r.pi_)
    {
        r.pi_ = nullptr;
    }

    /**
     * @brief 构造新的weak_ptr, 它共享r所管理的对象.
     *        若r不管理对象, 则*this亦不管理对象.
     *
     * @param r 被共享shared_ptr
     */
    weak_ptr(const shared_ptr<T>& r): pi_(r.pi_)
    {
        if (pi_ != nullptr) pi_->weak_add_ref();
    }

    /**
     * @brief 销毁weak_ptr对象. 对被管理对象不产生影响.
     */
    ~weak_ptr()
    {
        if (pi_ != nullptr) pi_->weak_release();
    }

    /**
     * @brief 以r所管理者替换被管理对象. 与r共享该对象. 
     *        若r不管理对象, 则*this亦不管理对象.
     *
     * @param r 与之共享对象的智能指针
     *
     * @return *this
     */
    weak_ptr& operator= (const weak_ptr& r)
    {
        /**
        if (this != &r) {
            if (r.pi_ != nullptr) r.pi_->weak_add_ref();
            if (pi_ != nullptr) pi_->weak_release();
            pi_ = r.pi_;
        }
        */
        this_type(r).swap(*this);
        return *this;
    }

    weak_ptr& operator= (weak_ptr&& r) noexcept
    {
        /**
        if (this != &r) {
            if (pi_ != nullptr) pi_->weak_release();
            pi_ = r.pi_;
            r.pi_ = nullptr;
        }
        */
        this_type(std::move(r)).swap(*this);
        return *this;
    }

    /**
     * @brief 以r所管理者替换被管理对象. 与r共享该对象. 
     *        若r不管理对象, 则*this亦不管理对象.
     *
     * @param r 与之共享对象的智能指针
     *
     * @return *this
     */
    weak_ptr& operator= (const shared_ptr<T>& r)
    {
        /**
        if (pi_ != r.pi_) {
            if (r.pi_ != nullptr) r.pi_->weak_add_ref();
            if (pi_ != nullptr) pi_->weak_release();
            pi_ = r.pi_;
        }
        */
        this_type(r).swap(*this);
        return *this;
    }

    /**
     * @brief 交换*this与r的内容
     *
     * @param r 要与之交换内容的智能指针
     */
    void swap(weak_ptr& r)
    {
        using std::swap;
        swap(this->pi_, r.pi_);
    }

    /**
     * @brief 释放被管理对象的所有权. 调用后*this不管理对象.
     */
    void reset()
    {
        /**
        if (pi_) {
            pi_->weak_release();
            pi_ = nullptr;
        }
        */
        this_type().swap(*this);
    }

    /**
     * @brief 返回共享被管理对象所有权的shared_ptr实例数量, 
     *        或0, 若被管理对象已被删除, 即*this为空.
     *
     * @return 在调用的瞬间共享被管理对象所有权的 shared_ptr 实例数量。
     *
     * @note expired()可能快于use_count(). 
     *       此函数固有地有不稳, 若被管理对象在可能创建并销毁shared_ptr
     *       副本的线程间共享: 则结果仅若匹配调用方线程所独占的副本数或零才可靠;
     *       任何其他值可能在能使用前就变得过时了.
     */
    long int use_count() const
    {
        return pi_ != nullptr ? pi_->use_count() : 0;
    }

    /**
     * @brief 等价于use_count() == 0. 
     *        可能仍未对被管理对象调用析构函数, 
     *        但此对象的析构已经临近（或可能已发生）.
     *
     * @return 若被管理对象已被删除则为true, 否则为false. 
     *
     * @note 若被管理对象在线程间共享, 则此函数内在地不可靠, 
     *       通常false结果可能在能用之前就变得过时. true结果可靠.
     */
    bool expired() const
    {
        return use_count() == 0;
    }

    /**
     * @brief 创建新的std::shared_ptr对象, 它共享被管理对象的所有权. 
     *        若无被管理对象, 即*this为空, 则返回亦为空的shared_ptr.
     *        等效地返回expired() ? shared_ptr<T>() : shared_ptr<T>(*this),
     *        原子地执行.
     *
     * @return 若std::weak_ptr::expired返回false,
     *         则为共享被占有对象所有权的shared_ptr. 
     *         否则返回默认构造的T类型的shared_ptr. 
     *
     * @note 此函数和std::shared_ptr的构造函数可能获得std::weak_ptr
     *       所指向的被管理对象的临时所有权. 
     *       区别是std::shared_ptr的构造函数在其std::weak_ptr为空时抛异常,
     *       而std::weak_ptr<T>::lock()构造空的std::shared_ptr<T>. 
     *
     */
    shared_ptr<T> lock() const
    {
        if (pi_ == nullptr || !pi_->add_ref_lock()) {
            return shared_ptr<T>();
        }

        return shared_ptr<T>(sp_counted_base_tag{}, pi_);
    }

    /**
     * @brief 以实现定义的基于拥有者（与基于值相反）顺序, 
     *        检查此weak_ptr是否先于r. 
     *        二个智能指针仅若都占有同一对象或均为空才比较相等,
     *        即使由get()获得的指针不同（例如因为它们指向同一对象中的不同子对象）.
     *
     * @param r 要比较的weak_ptr
     *
     * @return 若*this前于r则为true, 否则为false. 常见实现比较控制块的地址.
     */
    bool owner_before(const weak_ptr& r) const
    {
        return this->pi_ < r.pi_;
    }

    /**
     * @brief 以实现定义的基于拥有者（与基于值相反）顺序, 
     *        检查此weak_ptr是否先于r. 
     *        二个智能指针仅若都占有同一对象或均为空才比较相等,
     *        即使由get()获得的指针不同（例如因为它们指向同一对象中的不同子对象）.
     *
     * @param r 要比较的shared_ptr
     *
     * @return 若*this前于r则为true, 否则为false. 常见实现比较控制块的地址.
     */
    bool owner_before(const shared_ptr<T>& r) const
    {
        return this->pi_ < r.pi_;
    }
};

/**
 * @brief 为std::weak_ptr特化std::swap算法. 交换lhs与rhs的指针. 调用lhs.swap(rhs).
 *
 * @tparam T weak_ptr管理的对象类型
 * @param lhs 要交换内容的智能指针
 * @param rhs 要交换内容的智能指针
 */
template <typename T>
void swap(weak_ptr<T>& lhs, weak_ptr<T>& rhs)
{
    lhs.swap(rhs);
}

}   // namespace mini_stl

#endif
