cmake_minimum_required(VERSION 3.15)
project(algorithms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sorting INTERFACE)
target_include_directories(sorting INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

enable_testing()

add_subdirectory(tests)