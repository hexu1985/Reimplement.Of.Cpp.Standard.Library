file(GLOB MINI_STL_THREAD_LIB_SRC_LIST *.cpp)

add_library(mini_stl_recursive_timed_mutex SHARED ${MINI_STL_THREAD_LIB_SRC_LIST})
add_library(mini_stl_recursive_timed_mutex-static STATIC ${MINI_STL_THREAD_LIB_SRC_LIST})


