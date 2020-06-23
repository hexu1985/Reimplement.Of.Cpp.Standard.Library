/**
 * @file enable_shared_from_this.hpp
 * @brief 允许对象创建指代自身的shared_ptr
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-06-19
 */
#ifndef MINI_STL_ENABLE_SHARED_FROM_THIS_INC
#define MINI_STL_ENABLE_SHARED_FROM_THIS_INC

#include <assert.h>

namespace mini_stl {

template <typename T>
class shared_ptr;

template <typename T>
class weak_ptr;

/**
 * @brief 允许对象创建指代自身的shared_ptr
 *
 * @tparam T 共享对象的类型
 */
template <typename T>
class enable_shared_from_this {
private:
    mutable weak_ptr<T> weak_this_;

protected:
    /**
     * @brief 构造新的enable_shared_from_this 对象.
     *        值初始化私有 std::weak_ptr<T> 成员.
     */
    enable_shared_from_this() noexcept {}

    /**
     * @brief 构造新的enable_shared_from_this对象. 
     *        值初始化私有std::weak_ptr<T>成员.
     *
     * @note 不复制weak_this_变量
     */
    enable_shared_from_this(const enable_shared_from_this &) noexcept {}

    /**
     * @brief 不做任何事: 返回*this
     *
     * @return *this
     *
     * @note 私有的std::weak_ptr<T>成员不受此赋值运算符影响
     */
    enable_shared_from_this &operator =(const enable_shared_from_this &) noexcept
    {
        return *this;
    }

    /**
     * @brief 销毁enable_shared_from_this对象
     */
    ~enable_shared_from_this() noexcept {}

public:
    /**
     * @brief 返回与所有指代*this的std::shared_ptr共享*this所有权的std::shared_ptr<T>. 
     *        等效地执行std::shared_ptr<T>(weak_this), 
     *        其中weak_this是enable_shared_from_this的私有mutable std::weak_ptr<T>成员. 
     *
     * @return 与之前存在的std::shared_ptr共享*this所有权的 std::shared_ptr<T>. 
     */
    shared_ptr<T> shared_from_this()
    {
        shared_ptr<T> p(weak_this_);
        assert(p.get() == this);
        return p;
    }

    /**
     * @brief 返回与所有指代*this的std::shared_ptr共享*this所有权的std::shared_ptr<T>. 
     *        等效地执行std::shared_ptr<T>(weak_this), 
     *        其中weak_this是enable_shared_from_this的私有mutable std::weak_ptr<T>成员. 
     *
     * @return 与之前存在的std::shared_ptr共享*this所有权的 std::shared_ptr<T>. 
     */
    shared_ptr<const T> shared_from_this() const
    {
        shared_ptr<const T> p(weak_this_);
        assert(p.get() == this);
        return p;
    }

    // 只由构造shared_ptr时候调用
    void internal_accept_owner(const shared_ptr<T> *sp) const
    {
        if (weak_this_.expired()) {
            weak_this_ = shared_ptr<T>(*sp);
        }
    }
};

}   // namespace mini_stl

#endif
