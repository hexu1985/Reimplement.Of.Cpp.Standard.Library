/**
 * @file thread.hpp
 * @brief C++的线程类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-09-16
 */
#ifndef MINI_STL_THREAD_INC
#define MINI_STL_THREAD_INC

#include <memory>
#include <exception>
#include "thread_id.hpp"

namespace mini_stl {

/**
 * @brief 类 thread 表示单个执行线程
 */
class thread {
private:
	thread_id id_;	// 线程标识符

public:
	typedef	thread_id id;
	typedef	id::native_handle_type native_handle_type;

    /**
     * @brief 构造不表示线程的新 thread 对象。
     */
	thread() noexcept {}

    /**
     * @brief 复制构造函数被删除； thread 不可复制。没有二个 std::thread 对象可表示同一执行线程。
     */
	thread(const thread &) = delete;


    /**
     * @brief 赋值运算符被删除； thread 不可赋值。没有二个 std::thread 对象可表示同一执行线程。
     */
	thread &operator =(const thread &) = delete; 

    /**
     * @brief 构造新的 std::thread 对象并将它与执行线程关联。
     *
     * @tparam Fn 函数类型
     * @tparam ...Args 参数类型列表
     * @param fn 在新线程中执行的函数
     * @param ...args 参数列表
     */
	template <typename Fn, typename ...Args>
	explicit thread(Fn &&fn, Args &&...args)
	{
		create_thread(make_routine(std::bind(
			std::forward<Fn>(fn), std::forward<Args>(args)...)));
	}

    /**
     * @brief 移动构造函数。构造表示曾为 other 所表示的执行线程的 thread 对象。此调用后 other 不再表示执行线程。
     *
     * @param other 用以构造此 thread 的另一 thread 对象。
     */
	thread(thread &&other) noexcept: id_(other.id_) 
	{
		other.id_ = id(); 
	}

    /**
     * @brief 移动 thread 对象
     *
     * @param other 赋值给此 thread 对象的另一 thread 对象
     *
     * @return *this
     *
     * @note 若 *this 仍拥有关联的运行中进程（即 joinable() == true ），则调用 std::terminate() 。
     *       否则，赋值 other 的状态给 *this 并设置 other 为默认构造的状态。
     *       此调用后， this->get_id() 等于 other.get_id() 在调用前的值，而 other 不在表示执行的线程。
     */
	thread &operator =(thread &&other)
	{
		if (&other == this) {
			return *this;
		}

		if (joinable()) {
			std::terminate();
		}

		id_ = other.id_;
		other.id_ = id();

		return *this;
	}

    /**
     * @brief 析构 thread 对象，必须合并或分离底层线程。
     *
     * @note 若 *this 拥有关联线程（ joinable() == true ），则调用 std::terminate() 。
     */
	~thread() 
	{
		if (joinable()) {
			std::terminate(); 
		}
	}

    /**
     * @brief 检查 std::thread 对象是否标识活跃的执行线程。
     *
     * @return 具体而言，若 get_id() != std::thread::id() 则返回 true 。故默认构造的 thread 不可结合。
     */
	bool joinable() const noexcept { return (id_ != id()); }

    /**
     * @brief 返回线程的 id
     *
     * @return 标识与 *this 关联的线程的 std::thread::id 类型值。若无关联的线程，则返回默认构造的 std::thread::id 。
     */
	id get_id() const noexcept { return id_; }

    /**
     * @brief 返回实现定义的底层线程柄。
     *
     * @return 表示线程的实现定义柄类型。
     */
	native_handle_type native_handle() const { return id_.native_handle(); }

    /**
     * @brief 返回实现支持的并发线程数
     *
     * @return 返回实现支持的并发线程数。应该只把该值当做提示。
     */
	static unsigned hardware_concurrency();

    /**
     * @brief 阻塞当前线程直至 *this 所标识的线程结束其执行。
     *
     * @note *this 所标识的线程的完成同步于对应的从 join() 成功返回。
     *       *this 自身上不进行同步。同时从多个线程在同一 thread 对象上调用 join() 构成数据竞争，导致未定义行为。
     */
	void join();

    /**
     * @brief 从 thread 对象分离执行线程，允许执行独立地持续。
     *
     * @note 一旦该线程退出，则释放任何分配的资源。
     *       调用 detach 后 *this 不再占有任何线程。
     */
	void detach();

    /**
     * @brief 交换二个 thread 对象
     *
     * @param other 要与之交换的 thread
     *
     * @note 交换二个 thread 对象的底层柄。
     */
	void swap(thread &other) noexcept { id_.swap(other.id_); }

private:
    /**
     * @brief 例程基类
     */
	struct routine_base {
		virtual ~routine_base() {}
		virtual void run() = 0;
	};

    /**
     * @brief 例程子类
     *
     * @tparam Fn 封装
     */
	template <typename Fn>
	struct routine: public routine_base {
		routine(Fn &&fn): f_(std::forward<Fn>(fn)) {}

		virtual void run() { f_(); }

		Fn f_;
	};

    /**
     * @brief 创建例程对象
     *
     * @tparam Fn 函数对象类型
     * @param fn 函数
     *
     * @return 例程对象指针
     */
	template <typename Fn>
	routine<Fn> *make_routine(Fn &&fn)
	{
		return new routine<Fn>(std::forward<Fn>(fn));
	}

    /**
     * @brief 创建线程，在新线程里运行例程
     *
     * @param rtn 例程对象指针
     */
	void create_thread(routine_base *rtn);
};

inline 
void swap(mini_stl::thread &a, mini_stl::thread &b) noexcept
{
	a.swap(b);
}

}	// namespace mini_stl

#endif // HX_THREAD_H

