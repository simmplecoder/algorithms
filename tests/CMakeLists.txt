configure_file(testing_config.hpp.in testing_config.hpp)

foreach(file_name 
    quick_sort_test
    randomized_quicksort_test
    heap_sort_test
    binary_search_tree_test
    merge_sort_test
    counting_sort_test
    radix_sort_test
    comb_sort_test
    selection_strats
)
    set(sorting_executable_name ${file_name}_executable)
    add_executable(${sorting_executable_name} ${file_name}.cpp)
    if (NOT MSVC)
        target_compile_options(${sorting_executable_name} PRIVATE -Wall)
    else()
        target_compile_options(${sorting_executable_name} PRIVATE /W4)
    endif()
    target_link_libraries(${sorting_executable_name} PRIVATE sorting)
    # make sure compiler as well as intellisense can see produced testing_config.hpp
    target_include_directories(${sorting_executable_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME ${file_name} COMMAND ${sorting_executable_name})
endforeach()