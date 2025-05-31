cmake_minimum_required(VERSION 3.31)
project(associative_container)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(associative_container include/associative_container.hpp)
target_include_directories(associative_container PUBLIC ./include)

set_target_properties(
        associative_container PROPERTIES
        VERSION 1.0
        DESCRIPTION "associative container interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_subdirectory(search_tree)
add_subdirectory(hash_table)