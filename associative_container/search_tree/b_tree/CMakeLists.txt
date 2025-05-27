cmake_minimum_required(VERSION 3.31)
project(b_tree)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(b_tree ${HEADERS})
target_include_directories(b_tree PUBLIC ./include)

set_target_properties(
        b_tree PROPERTIES
        VERSION 1.0
        DESCRIPTION "b tree interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)