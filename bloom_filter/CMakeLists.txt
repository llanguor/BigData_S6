cmake_minimum_required(VERSION 3.31)
project(bloom_filter)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
)

add_library(bloom_filter ${HEADERS} ${SOURCES})
target_include_directories(bloom_filter PUBLIC ./include)

set_target_properties(
        bloom_filter PROPERTIES
        VERSION 1.0
        DESCRIPTION "associative container interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_subdirectory(builder)
add_subdirectory(hasher)