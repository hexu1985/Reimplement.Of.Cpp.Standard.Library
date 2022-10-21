/**
 * @file thread_id.hpp
 * @brief thread::id实现
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2021-07-12
 */
#ifndef MINI_STL_THREAD_ID_INC
#define MINI_STL_THREAD_ID_INC

#include <windows.h>

#include <iosfwd>
#include <cstdint>
#include <utility>
#include <functional>

namespace Hx {

/**
 * @brief 表示线程的 id
 */
struct thread_id {
    HANDLE handle = nullptr; // native thread handle
    unsigned  id = 0;           // thread id

    typedef HANDLE native_handle_type;

    /**
     * @brief 构造不表示线程的 id
     */
    thread_id() = default;

    /**
     * @brief 通过pthread_t类型的id构造一个线程的id
     *
     * @param tid pthread_t类型的id
     */
    thread_id(HANDLE thandle, unsigned tid) 
    {
      handle = thandle;
      id = tid; 
    }

    /**
     * @brief 比较两个 thread::id 对象
     *
     * @param lhs 要比较的线程标识符 - 左操作数
     * @param rhs 要比较的线程标识符 - 右操作数
     *
     * @return 比较结果
     */
    friend bool operator==(const thread_id& lhs, const thread_id& rhs);
    friend bool operator!=(const thread_id& lhs, const thread_id& rhs);
    friend bool operator<(const thread_id& lhs, const thread_id& rhs);
    friend bool operator<=(const thread_id& lhs, const thread_id& rhs);
    friend bool operator>(const thread_id& lhs, const thread_id& rhs);
    friend bool operator>=(const thread_id& lhs, const thread_id& rhs);

    /**
     * @brief 返回底层实现定义的线程句柄
     *
     * @return HANDLE类型的handle
     */
    native_handle_type native_handle() const { return handle; }

    /**
     * @brief 交换两个线程的id
     *
     * @param other 被交换对象
     */
    void swap(thread_id& other)
    {
        using std::swap;
        swap(id, other.id);
        swap(handle, other.handle);
    }
};

inline bool operator==(const thread_id& lhs, const thread_id& rhs)
{
    return (lhs.id == rhs.id) && (lhs.handle == rhs.handle);
}

inline bool operator!=(const thread_id& lhs, const thread_id& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(const thread_id& lhs, const thread_id& rhs)
{
    if (lhs.id < rhs.id) {
        return true;
    } else if (lhs.id == rhs.id) {
        return lhs.handle < lhs.handle;
    } else {
        return false;
    }
}

inline bool operator<=(const thread_id& lhs, const thread_id& rhs)
{
    return !(rhs < lhs);
}

inline bool operator>(const thread_id& lhs, const thread_id& rhs)
{
    return (rhs < lhs);
}

inline bool operator>=(const thread_id& lhs, const thread_id& rhs)
{
    return !(lhs < rhs);
}

/**
 * @brief 线程标识符 id 的文本表示到输出流 os
 *
 * @param os 要插入数据的输出流
 * @param id 线程标识符
 *
 * @return 输出流os的引用
 */
template <typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(
    std::basic_ostream<charT, traits>& os, const thread_id& id)
{
    os << id.id;
    return os;
}

}    // namespace Hx

namespace std {

/**
 * @brief std::hash 对 thread_id 类的模板特化允许用户获得线程标识符的哈希。
 */
template <> struct hash<Hx::thread_id>
{
    size_t operator()(const Hx::thread_id& id) const noexcept
    {
        return std::hash<unsigned>()(id.id);
    }
};

}    // namespace std

#endif

