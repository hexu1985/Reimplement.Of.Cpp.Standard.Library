cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(recipe-02 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

file(GLOB src_files ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach (src_file ${src_files})
    get_filename_component(exe_file ${src_file} NAME_WE)
    set(exe_file ${exe_file})
    add_executable(${exe_file} ${src_file})
endforeach()


