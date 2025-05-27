cmake_minimum_required(VERSION 3.31)
project(search_tree)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(search_tree include/search_tree.hpp)
target_include_directories(search_tree PUBLIC ./include)
target_link_libraries(search_tree PUBLIC associative_container)

set_target_properties(
        search_tree PROPERTIES
        VERSION 1.0
        DESCRIPTION "search tree interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_subdirectory(b_tree)