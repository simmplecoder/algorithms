cmake_minimum_required(VERSION 3.10)
project(algorithms)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

add_executable(quick_sort quick_sort_test.cpp quick_sort.hpp)
add_test(NAME quick_sort_test COMMAND quick_sort)

add_executable(heap_sort heap_sort.hpp heap_sort_test.cpp)
add_test(NAME heap_sort_test COMMAND heap_sort)

add_executable(binary_search_tree binary_search_tree_test.cpp binary_tree_iterator.hpp binary_search_tree.hpp)
add_test(NAME binary_search_tree_test COMMAND binary_search_tree)

add_executable(merge_sort merge_sort.hpp merge_sort_test.cpp)
add_test(NAME merge_sort_test COMMAND merge_sort)

add_executable(counting_sort counting_sort_test.cpp)
add_test(NAME counting_sort_test COMMAND counting_sort)

add_executable(benchmarkv2 benchmark/benchmarkv2.cpp)

add_executable(radix_sort radix_sort_test.cpp)