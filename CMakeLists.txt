cmake_minimum_required(VERSION 2.8)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(Eigen3 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/transform/eigen)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(test_golden_search test/golden_search.cpp)
add_executable(eigen_test test/eigentest.cpp)
add_executable(static_transform test/static_transform.cpp)
add_executable(multidiff_transform test/multidiff_transform.cpp)
add_executable(debug_multidiff_transform test/debug_multidiff_transform.cpp)

