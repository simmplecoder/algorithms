configure_file(testing_config.hpp.in testing_config.hpp)
enable_testing()

foreach(FILE_NAME 
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
    set(SORTING_EXECUTABLE_NAME ${FILE_NAME}_executable)
    add_executable(${SORTING_EXECUTABLE_NAME} ${FILE_NAME}.cpp)
    if (NOT MSVC)
        target_compile_options(${SORTING_EXECUTABLE_NAME} PRIVATE -Wall)
    else()
        target_compile_options(${SORTING_EXECUTABLE_NAME} PRIVATE /W4)
    endif()
    target_link_libraries(${SORTING_EXECUTABLE_NAME} PRIVATE sorting)
    # make sure compiler as well as intellisense can see produced testing_config.hpp
    target_include_directories(${SORTING_EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME ${FILE_NAME} COMMAND ${SORTING_EXECUTABLE_NAME})
endforeach()