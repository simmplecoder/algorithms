cmake_minimum_required(VERSION 3.15)
project(sorting)

option(SORTING_ENABLE_TESTING "enable test targets" OFF)
option(SORTING_ENABLE_THOROUGH_TESTING "enale more thorough testing" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sorting INTERFACE)
target_include_directories(sorting INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if (SORTING_ENABLE_TESTING)
    add_subdirectory(tests)
endif()