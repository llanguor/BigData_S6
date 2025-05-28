cmake_minimum_required(VERSION 3.31)
project(hasher_adapter)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(hasher_adapter ${HEADERS})
target_include_directories(hasher_adapter PUBLIC ./include)

set_target_properties(
        hasher_adapter PROPERTIES
        VERSION 1.0
        DESCRIPTION "hasher_adapter library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)