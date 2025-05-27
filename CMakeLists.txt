cmake_minimum_required(VERSION 3.31)
project(BigData_S6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(associative_container)
add_executable(B_Tree main_b_tree.cpp)
target_link_libraries(
        B_Tree
        PRIVATE
        associative_container
        search_tree
        b_tree)

add_subdirectory(bloom_filter)
add_executable(Bloom_Filter main_bloom_filter.cpp)
target_link_libraries(
        Bloom_Filter
        PRIVATE
        bloom_filter
        bloom_filter_builder
        hasher
        hasher_md5)

