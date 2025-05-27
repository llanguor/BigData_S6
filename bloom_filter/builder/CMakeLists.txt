cmake_minimum_required(VERSION 3.31)
project(bloom_filter_builder)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
)

add_library(bloom_filter_builder ${HEADERS} ${SOURCES})
target_include_directories(bloom_filter_builder PUBLIC ./include)

set_target_properties(
        bloom_filter_builder PROPERTIES
        VERSION 1.0
        DESCRIPTION "bloom_filter_builder library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)