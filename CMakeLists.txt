cmake_minimum_required(VERSION 3.15)
project(sorting)

include(GNUInstallDirs)
option(SORTING_ENABLE_TESTING "enable test targets" OFF)
option(SORTING_ENABLE_THOROUGH_TESTING "enale more thorough testing" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sorting INTERFACE)
target_include_directories(sorting INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (SORTING_ENABLE_TESTING)
    add_subdirectory(tests)
endif()

set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)

install(TARGETS sorting
    EXPORT sortingTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT sortingTargets
    NAMESPACE sorting::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/sorting"
)

install(
    DIRECTORY
        include/
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES
        cmake/sortingConfig.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/sorting
)