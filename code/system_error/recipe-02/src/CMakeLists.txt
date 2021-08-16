file(GLOB MINI_STL_SYSTEM_ERROR_LIB_SRC_LIST *.cpp)

add_library(mini_stl_system_error SHARED ${MINI_STL_SYSTEM_ERROR_LIB_SRC_LIST})
add_library(mini_stl_system_error-static STATIC ${MINI_STL_SYSTEM_ERROR_LIB_SRC_LIST})


