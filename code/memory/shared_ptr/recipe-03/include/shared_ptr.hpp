/**
 * @file shared_ptr.hpp
 * @brief 拥有共享对象所有权语义的智能指针
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-04-03
 */
#ifndef MINI_STL_SHARED_PTR_INC
#define MINI_STL_SHARED_PTR_INC

#include <iostream>
#include <utility>
#include <cstddef>
#include "sp_counted_impl.hpp"
#include "bad_weak_ptr.hpp"

namespace Hx {

template <typename T>
class shared_ptr;

template <typename T>
class weak_ptr;

template <typename T>
class enable_shared_from_this;

template <typename T>
inline void sp_enable_shared_from_this(const shared_ptr<T>* sp, const enable_shared_from_this<T>* pe)
{
	if (pe != nullptr) {
		pe->internal_accept_owner(sp);
	}
}

// SFINAE: Substitution Failure Is Not An Error
// 默认匹配函数
inline void sp_enable_shared_from_this(...)
{
}

/**
 * @brief 拥有共享对象所有权语义的智能指针
 *
 * @tparam T 共享对象的类型
 */
template <typename T>
class shared_ptr {
public:
    typedef T element_type;

private:
    sp_counted_base* pi_ = nullptr;
    element_type* px_ = nullptr;

    typedef shared_ptr<T> this_type;

    template <typename> friend class shared_ptr;
    template <typename> friend class weak_ptr;

    // for make_shared call only
    shared_ptr(sp_counted_base_tag, sp_counted_base* pi): pi_(pi), px_(static_cast<T*>(pi->get_pointer())) 
    {
        sp_enable_shared_from_this(this, get());
    }

    template <typename U, typename... Args>
    friend shared_ptr<U> make_shared(Args&&... args);

public:
    /**
     * @brief 构造无被管理对象的shared_ptr, 即空shared_ptr
     */
    shared_ptr() {}
    shared_ptr(std::nullptr_t) {}

    /**
     * @brief 构造shared_ptr, 管理ptr所指向的对象.
     *
     * @tparam Y 共享对象的类型
     * @param ptr 指向共享对象的指针
     *
     * @note  Y* 必须可转换为 T*
     */
    template <typename Y>
    explicit shared_ptr(Y* ptr): px_(ptr)
    {
        if (!ptr) return;   // 处理shared_ptr(nullptr)的情况

        try 
        {
            pi_ = new sp_counted_impl_p<Y>(ptr);
        }
        catch (...) 
        {
            delete ptr;
            throw;
        }
        sp_enable_shared_from_this(this, ptr);
    }

    /**
     * @brief 构造shared_ptr, 管理ptr所指向的对象,
     *        当共享引用计数降到0时, 通过deleter释放指针ptr
     *
     * @tparam Y 共享对象的类型
     * @tparam Deleter  自定义的deleter的类型
     * @param ptr 指向共享对象的指针
     * @param d deleter的引用
     */
    template <typename Y, typename Deleter>
    shared_ptr(Y* ptr, Deleter d): px_(ptr)
    {
        try 
        {
            pi_ = new sp_counted_impl_pd<Y*, Deleter>(ptr, d);
        }
        catch (...)
        {
            d(ptr);    // delete ptr
            throw;
        }
        sp_enable_shared_from_this(this, ptr);
    }

    /**
     * @brief 若*this占有对象且它是最后一个占有该对象的shared_ptr, 
     *        则通过占有的删除器销毁对象.
     *        析构后, 与*this共享所有权的智能指针若存在, 
     *        则报告比先前值少一的use_count(). 
     */
    ~shared_ptr()
    {
        if (pi_ != nullptr) pi_->release();
    }

    /**
     * @brief 构造shared_ptr, 共享r所管理对象的所有权.
     *        若r不管理对象, 则*this亦不管理对象.
     *
     * @param r 被共享shared_ptr
     */
    shared_ptr(const shared_ptr& r): pi_(r.pi_), px_(r.px_)
    {
        if (pi_ != nullptr) pi_->add_ref_copy();
    }

    /**
     * @brief 构造 shared_ptr, 共享r所管理对象的所有权.
     *        若r不管理对象, 则*this 亦不管理对象. 
     *
     * @tparam Y 共享对象的类型
     * @param r 被共享shared_ptr
     *
     * @note 若Y*不可隐式转换为T*, 则模板重载不参与重载决议。
     */
    template <typename Y>
    shared_ptr(const shared_ptr<Y>& r): pi_(r.pi_), px_(r.px_)
    {
        if (pi_ != nullptr) pi_->add_ref_copy();
    }

    /**
     * @brief 从r移动构造shared_ptr.
     *        构造后, *this含r先前状态的副本, 而r为空且其存储的指针为空.
     *
     * @param r 从它获得所有权的另一智能指针
     */
    shared_ptr(shared_ptr&& r) noexcept: pi_(r.pi_), px_(r.px_)
    {
        r.pi_ = nullptr;
        r.px_ = nullptr;
    } 

    template <typename Y>
    shared_ptr(shared_ptr<Y>&& r) noexcept: pi_(r.pi_), px_(r.px_)
    {
        r.pi_ = nullptr;
        r.px_ = nullptr;
    } 
	
    /**
     * @brief 构造shared_ptr, 共享r所管理对象的所有权.
     *
     * @tparam Y 共享对象的类型
     * @param r 被共享weak_ptr
     *
     * @note Y*必须可隐式转换为T*
     */
    template <typename Y>
    explicit shared_ptr(const weak_ptr<Y>& r): pi_(r.pi_), px_(r.px_)
    {
        if (pi_ == nullptr || !pi_->add_ref_lock()) {
            throw bad_weak_ptr{};
        }
    }

    /**
     * @brief 别名使用构造函数: 构造 shared_ptr, 与r的初始值共享所有权信息, 但保有无关且不管理的指针ptr. 
     *
     * @tparam Y 共享对象的类型
     * @param r 被共享weak_ptr
     * @param ptr 别名指针
     *
     * @note 若此shared_ptr是离开作用域的组中的最后者, 则它将调用最初r所管理对象的析构函数.
     *       然而, 在此shared_ptr上调用get()将始终返回ptr的副本. 
     *       程序员负责确保只要此shared_ptr存在, 此ptr就保持合法.
     */
    template <typename Y>
    shared_ptr(const shared_ptr<Y>& r, element_type* ptr): pi_(r.pi_), px_(ptr)
    {
        if (pi_ != nullptr) pi_->add_ref_copy();
    }

    /**
     * @brief 赋值运算符, 以r所管理者替换被管理对象.
     *        若*this已占有对象且它是最后一个占有该对象的shared_ptr, 
     *        且r与*this不相同, 则通过占有的删除器销毁对象.
     *
     * @param r 要获得共享所有权的另一智能指针
     *
     * @return *this
     */
    shared_ptr& operator= (const shared_ptr& r)
    {
        /**
        if (this != &r) {
            if (r.pi_ != nullptr) r.pi_->add_ref_copy();
            if (pi_ != nullptr) pi_->release();
            pi_ = r.pi_;
            px_ = r.px_;
        }
        */
        this_type(r).swap(*this);
        return *this;
    }

    /**
     * @brief 赋值运算符, 以r所管理者替换被管理对象.
     *        若*this已占有对象且它是最后一个占有该对象的shared_ptr, 
     *        且r与*this不相同, 则通过占有的删除器销毁对象.
     *
     * @tparam Y 共享对象的类型
     * @param r 要获得共享所有权的另一智能指针
     *
     * @return *this
     */
    template <typename Y>
    shared_ptr& operator= (const shared_ptr<Y>& r)
    {
        this_type(r).swap(*this);
        return *this;
    }

    /**
     * @brief 从r移动赋值shared_ptr.
     *        赋值后, *this含有先前r状态的副本, 而r为空,
     *        等价于shared_ptr<T>(std::move(r)).swap(*this) 。
     *
     * @param r 要获得所有权的另一智能指针
     *
     * @return *this
     */
    shared_ptr& operator= (shared_ptr&& r) noexcept
    {
        /**
        if (this != &r) {
            if (pi_ != nullptr) pi_->release();
            pi_ = r.pi_;
            px_ = r.px_;
            r.pi_ = nullptr;
            r.px_ = nullptr;
        }
        */
        this_type(std::move(r)).swap(*this);
        return *this;
    }

    /**
     * @brief 从r移动赋值shared_ptr.
     *        赋值后, *this含有先前r状态的副本, 而r为空,
     *        等价于shared_ptr<T>(std::move(r)).swap(*this) 。
     *
     * @tparam Y 共享对象的类型
     * @param r 要获得所有权的另一智能指针
     *
     * @return *this
     */
    template <typename Y>
    shared_ptr& operator= (shared_ptr<Y>&& r) noexcept
    {
        this_type(std::move(r)).swap(*this);
        return *this;
    }

    /**
     * @brief 交换*this与r的内容
     *
     * @param r 要与之交换内容的智能指针
     */
    void swap(shared_ptr& r)
    {
        using std::swap;
        swap(this->pi_, r.pi_);
        swap(this->px_, r.px_);
    }

    /**
     * @brief 释放被管理对象的所有权, 若存在. 
     *        调用后, *this不管理对象. 
     *        等价于shared_ptr().swap(*this);
     */
    void reset()
    {
        /**
        if (pi_) {
            pi_->release();
            pi_ = nullptr;
            px_ = nullptr;
        }
        */
        this_type().swap(*this);
    }

    /**
     * @brief 以ptr所指向对象替换被管理对象.
     *        以delete表达式为删除器.
     *        等价于shared_ptr<T>(ptr).swap(*this);
     *
     * @tparam Y 共享对象的类型
     * @param ptr 指向要取得所有权的对象的指针
     *
     * @note 合法的delete表达式必须可用, 即delete ptr必须为良式, 拥有良好定义行为且不抛任何异常.
     */
    template <typename Y>
    void reset(Y* ptr)
    {
        this_type(ptr).swap(*this);
    }

    /**
     * @brief 以ptr所指向对象替换被管理对象.
     *        以指定的删除器d为删除器.
     *        等价于shared_ptr<T>(ptr, d).swap(*this);
     *
     * @tparam Y 共享对象的类型
     * @tparam Deleter deleter类型
     * @param ptr 指向要取得所有权的对象的指针
     * @param d 为删除对象而存储的删除器
     *
     * @note  Deleter必须对T类型可调用, 即d(ptr)必须为良构, 拥有良好定义行为且不抛任何异常.
     *        Deleter必须可复制构造(CopyConstructible), 且其复制构造函数和析构函数必须不抛异常.
     */
    template <typename Y, typename Deleter>
    void reset(Y* ptr, Deleter d)
    {
        this_type(ptr, d).swap(*this);
    }

    /**
     * @brief 返回存储的指针
     *
     * @return 存储的指针
     */
    element_type* get() const
    {
        return px_;
    }

    /**
     * @brief 解引用所存储的指针
     *
     * @return 解引用存储的指针所得的结果, 即*get()
     *
     * @note 若存储的指针为空，则行为未定义
     */
    element_type& operator* () const { return *get(); }

    /**
     * @brief 解引用所存储的指针
     *
     * @return 存储的指针, 即get()
     *
     * @note 若存储的指针为空，则行为未定义
     */
    element_type* operator-> () const { return get(); }

    /**
     * @brief 返回管理当前对象的不同shared_ptr实例(包含this)数量.
     *        若无管理对象, 则返回0.
     *
     * @return 管理当前对象的shared_ptr实例数量, 或若无被管理对象则为0.
     *
     * @note 多线程环境下, use_count返回的值是近似的(典型实现使用memory_order_relaxed加载)
     */
    long int use_count() const
    {
        return pi_ != nullptr ? pi_->use_count() : 0;
    }

    /**
     * @brief 检查*this是否管理当前对象的仅有shared_ptr实例, 即是否use_count() == 1
     *
     * @return 若*this否管理当前对象的仅有shared_ptr实例则为true, 否则为false. 
     *
     * @note 此函数于C++17中被弃用并于C++20中移除, 因为use_count在多线程环境中只是估计
     */
    bool unique() const
    {
        return use_count() == 1;
    }

    /**
     * @brief 检查*this是否存储非空指针, 即是否有get() != nullptr 
     *
     * @return 若*this存储非空指针则为true, 否则为false. 
     */
    explicit operator bool() const
    {
        return get() != nullptr;
    }

    /**
     * @brief 访问*this的删除器. 
     *        若共享指针*this占有无cv限定Deleter类型的删除器(例如, 若它以接收删除器为参数的构造函数之一创建), 
     *        则返回指向删除器的指针. 否则, 返回空指针.
     *
     * @return 指向被占有删除器的指针或nullptr
     */
    void* get_deleter() const
    {
        return (this->pi_ != nullptr ? this->pi_->get_deleter() : 0);
    }

    /**
     * @brief 以实现定义的基于拥有者(与基于值相反)顺序, 检查此shared_ptr是否先于r. 
     *        二个智能指针仅若都占有同一对象或均为空才比较相等, 
     *        即使由get()获得的指针不同(例如因为它们指向同一对象中的不同子对象)
     *        此顺序用于令共享和弱指针可用作关联容器中的关键，通常经由 std::owner_less.
     *
     * @tparam Y 共享对象的类型
     * @param r 要比较的 std::shared_ptr
     *
     * @return 若*this前于r则为true, 否则为false. 常见实现比较控制块的地址.
     */
    template <typename Y>
    bool owner_before(const shared_ptr<Y>& r) const
    {
        return this->pi_ < r.pi_;
    }

    /**
     * @brief 以实现定义的基于拥有者(与基于值相反)顺序, 检查此shared_ptr是否先于r. 
     *        二个智能指针仅若都占有同一对象或均为空才比较相等, 
     *        即使由get()获得的指针不同(例如因为它们指向同一对象中的不同子对象)
     *        此顺序用于令共享和弱指针可用作关联容器中的关键，通常经由 std::owner_less. 
     *
     * @tparam Y 共享对象的类型
     * @param r 要比较的 std::weak_ptr
     *
     * @return 若*this前于r则为true, 否则为false. 常见实现比较控制块的地址.
     */
    template <typename Y>
    bool owner_before(const weak_ptr<Y>& r) const
    {
        return this->pi_ < r.pi_;
    }
};

/**
 * @brief 比较二个shared_ptr<T>对象
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return lhs.get() == rhs.get()
 */
template <typename T>
bool operator== (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs)
{
    return lhs.get() == rhs.get();
}

/**
 * @brief 比较二个shared_ptr<T>对象
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return !(lhs == rhs)
 */
template <typename T>
bool operator!= (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @brief 比较二个shared_ptr<T>对象
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return std::less<V>()(lhs.get(), rhs.get()), 
 *         其中V是std::shared_ptr<T>::element_type*与std::shared_ptr<U>::element_type*的合成指针类型
 */
template <typename T>
bool operator< (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs)
{
    return lhs.get() < rhs.get();
}

/**
 * @brief 比较二个shared_ptr<T>对象
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return rhs < lhs
 */
template <typename T>
bool operator> (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs)
{
    return rhs < lhs;
}

/**
 * @brief 比较二个shared_ptr<T>对象
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return !(rhs < lhs)
 */
template <typename T>
bool operator<= (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @brief 比较二个shared_ptr<T>对象
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return !(lhs < rhs)
 */
template <typename T>
bool operator>= (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param std::nullptr_t nullptr类型
 *
 * @return !lhs
 */
template <typename T>
bool operator== (const shared_ptr<T>& lhs, std::nullptr_t)
{
    return !lhs;
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param std::nullptr_t nullptr类型
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return !rhs
 */
template <typename T>
bool operator== (std::nullptr_t, const shared_ptr<T>& rhs)
{
    return !rhs;
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param std::nullptr_t nullptr类型
 *
 * @return (bool) lhs
 */
template <typename T>
bool operator!= (const shared_ptr<T>& lhs, std::nullptr_t)
{
    return (bool) lhs;
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param std::nullptr_t nullptr类型
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return (bool) rhs
 */
template <typename T>
bool operator!= (std::nullptr_t, const shared_ptr<T>& rhs)
{
    return (bool) rhs;
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param std::nullptr_t nullptr类型
 *
 * @return std::less<std::shared_ptr<T>::element_type*>()(lhs.get(), nullptr)
 */
template <typename T>
bool operator< (const shared_ptr<T>& lhs, std::nullptr_t)
{
    return lhs.get() < nullptr;
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param std::nullptr_t nullptr类型
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return std::less<std::shared_ptr<T>::element_type*>()(nullptr, rhs.get())
 */
template <typename T>
bool operator< (std::nullptr_t, const shared_ptr<T>& rhs)
{
    return nullptr < rhs.get();
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param std::nullptr_t nullptr类型
 *
 * @return nullptr < lhs
 */
template <typename T>
bool operator> (const shared_ptr<T>& lhs, std::nullptr_t)
{
    return (nullptr < lhs);
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param std::nullptr_t nullptr类型
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return rhs < nullptr
 */
template <typename T>
bool operator> (std::nullptr_t, const shared_ptr<T>& rhs)
{
    return (rhs < nullptr);
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param std::nullptr_t nullptr类型
 *
 * @return !(nullptr < lhs)
 */
template <typename T>
bool operator<= (const shared_ptr<T>& lhs, std::nullptr_t)
{
    return !(nullptr < lhs);
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param std::nullptr_t nullptr类型
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return !(rhs < nullptr)
 */
template <typename T>
bool operator<= (std::nullptr_t, const shared_ptr<T>& rhs)
{
    return !(rhs < nullptr);
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param lhs 要比较的左侧shared_ptr
 * @param std::nullptr_t nullptr类型
 *
 * @return !(lhs < nullptr)
 */
template <typename T>
bool operator>= (const shared_ptr<T>& lhs, std::nullptr_t)
{
    return !(lhs < nullptr);
}

/**
 * @brief 将shared_ptr<T>与空指针比较.
 *
 * @param std::nullptr_t nullptr类型
 * @param rhs 要比较的右侧shared_ptr
 *
 * @return !(nullptr < rhs)
 */
template <typename T>
bool operator>= (std::nullptr_t, const shared_ptr<T>& rhs)
{
    return !(nullptr < rhs);
}

/**
 * @brief 插入存储于ptr的指针值到输出流os中.
 *        等价于os << ptr.get() 
 *
 * @param os 要插入ptr到的 std::basic_ostream
 * @param ptr 被插入到 os 的数据
 *
 * @return 
 */
template <typename charT, typename traits, typename T>
std::basic_ostream<charT, traits>& operator<< (std::basic_ostream<charT, traits>& os, 
    const shared_ptr<T>& ptr)
{
    os << ptr.get();
    return os;
}

/**
 * @brief 以args为T的构造函数参数列表, 构造T类型对象并将它包装于std::shared_ptr. 
 *        对象如同用表达式::new (pv) T(std::forward<Args>(args)...)构造.
 *
 * @tparam T shared_ptr管理的对象类型
 * @tparam ...Args 可变参数类型列表
 * @param ...args 将用以构造T实例的参数列表
 *
 * @return 类型T实例的std::shared_ptr
 */
template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args)
{
    /*
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
    */
    return shared_ptr<T>(sp_counted_base_tag{}, new sp_counted_impl<T>(std::forward<Args>(args)...));
}

/**
 * @brief 为std::shared_ptr特化std::swap算法. 交换lhs与rhs的指针. 调用lhs.swap(rhs). 
 *
 * @tparam T shared_ptr管理的对象类型
 * @param lhs 要交换内容的智能指针
 * @param rhs 要交换内容的智能指针
 */
template <typename T>
void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs)
{
    lhs.swap(rhs);
}

/**
 * @brief 访问p的删除器. 
 *        若共享指针p占有无cv限定Deleter类型的删除器(例如, 若它以接收删除器为参数的构造函数之一创建),
 *        则返回指向删除器的指针. 否则. 返回空指针.
 *
 * @tparam Deleter 删除器类型
 * @tparam T shared_ptr管理的对象类型
 * @param p 需要访问其删除器的共享指针
 *
 * @return 指向被占有删除器的指针或nullptr. 只要至少还有一个shared_ptr实例占有返回的指针, 它就合法.
 */
template <typename Deleter, typename T>
Deleter* get_deleter(const shared_ptr<T>& p)
{
    return static_cast<Deleter*>(p.get_deleter());
}

/**
 * @brief 创建std::shared_ptr的新实例, 其存储指针从r的存储指针用转型表达式获得.
 *
 * @tparam T to shared_ptr的共享对象类型
 * @tparam Y from shared_ptr的共享对象类型
 * @param r 要转换的指针 
 *
 * @return to shared_ptr
 */
template <typename T, typename Y>
shared_ptr<T> static_pointer_cast(const shared_ptr<Y>& r)
{
    (void) static_cast<T*>(static_cast<Y*>(0));

    typedef typename shared_ptr<T>::element_type E;

    E* p = static_cast<E*>(r.get());
    return shared_ptr<T>(r, p);
}

/**
 * @brief 创建std::shared_ptr的新实例, 其存储指针从r的存储指针用转型表达式获得.
 *
 * @tparam T to shared_ptr的共享对象类型
 * @tparam Y from shared_ptr的共享对象类型
 * @param r 要转换的指针 
 *
 * @return to shared_ptr
 */
template <typename T, typename Y>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<Y>& r)
{
    (void) dynamic_cast<T*>(static_cast<Y*>(0));

    typedef typename shared_ptr<T>::element_type E;

    E* p = dynamic_cast<E*>(r.get());
    return p ? shared_ptr<T>(r, p) : shared_ptr<T>();
}

/**
 * @brief 创建std::shared_ptr的新实例, 其存储指针从r的存储指针用转型表达式获得.
 *
 * @tparam T to shared_ptr的共享对象类型
 * @tparam Y from shared_ptr的共享对象类型
 * @param r 要转换的指针 
 *
 * @return to shared_ptr
 */
template <typename T, typename Y>
shared_ptr<T> const_pointer_cast(const shared_ptr<Y>& r)
{
    (void) const_cast<T*>(static_cast<Y*>(0));

    typedef typename shared_ptr<T>::element_type E;

    E* p = const_cast<E*>(r.get());
    return shared_ptr<T>(r, p);
}

}   // namespace Hx

#endif
