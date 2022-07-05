file(GLOB MINI_STL_THREAD_LIB_SRC_LIST *.cpp)

add_library(Hx_thread SHARED ${MINI_STL_THREAD_LIB_SRC_LIST})
add_library(Hx_thread-static STATIC ${MINI_STL_THREAD_LIB_SRC_LIST})


