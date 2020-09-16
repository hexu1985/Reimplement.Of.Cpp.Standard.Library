#include "thread.hpp"
#include <system_error>
#include "pthread.h"

namespace {

void *pthread_function(void *arg)
{
    auto routine = reinterpret_cast<mini_stl::thread::routine_base *>(arg);
    try 
    {
        routine->run();
    } 
    catch (...) 
    {
        delete routine;
        throw;
    }
    delete routine;
    return NULL;
}

}   // namespace

namespace mini_stl {

void thread::join()
{
    if (!joinable()) {
        throw std::system_error(EINVAL, std::system_category(), __func__);
    }

    int error = pthread_join(id_.native_handle(), NULL);
    if (error != 0) {
        throw std::system_error(error, std::system_category(), __func__);
    }
    id_ = id();

}

void thread::create_thread(routine_base *rtn)
{
	pthread_t tid;
	int error = pthread_create(&tid, NULL, &pthread_function, rtn);
	if (error != 0) {
		delete rtn;
		throw std::system_error(error, std::system_category(), __func__);
	}
	id_ = tid;
}

}   // namespace mini_stl


