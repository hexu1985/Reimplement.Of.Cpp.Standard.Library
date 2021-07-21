#include "thread.hpp"
#include <system_error>

static unsigned WINAPI ThreadFunc(void *arg)
{
    auto routine = reinterpret_cast<mini_stl::thread::routine_base*>(arg);
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
    return 0;
}

namespace mini_stl {

unsigned thread::hardware_concurrency()
{
    SYSTEM_INFO info={{0}};
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
}

void thread::join()
{
    if (!joinable()) {
        throw std::system_error(EINVAL, std::system_category(), __func__);
    }

    DWORD wr;
    if ((wr = WaitForSingleObject(id_.native_handle(), INFINITE)) == WAIT_FAILED) {
        throw std::system_error(GetLastError(), std::system_category(), __func__);
    }

    id_ = id();
}

void thread::detach()
{
    if (!joinable()) {
        throw std::system_error(EINVAL, std::system_category(), __func__);
    }

    CloseHandle(id_.native_handle());
    id_ = id();
}

void thread::create_thread(routine_base *rtn)
{
    HANDLE hThread;
    unsigned threadID;
    hThread = (HANDLE) _beginthreadex(NULL, 0, ThreadFunc, (void *) rtn, 0, &threadID);
    if (hThread == 0) {
        delete rtn;
        throw std::system_error(GetLastError(), std::system_category(), __func__);
    }
    id_ = thread_id(hThread, threadID);
}

}   // namespace mini_stl


