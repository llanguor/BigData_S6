cmake_minimum_required(VERSION 3.31)
project(bloom_filter)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(bloom_filter ${HEADERS})
target_include_directories(bloom_filter PUBLIC ./include)
target_link_libraries(bloom_filter PUBLIC hasher)

set_target_properties(
        bloom_filter PROPERTIES
        VERSION 1.0
        DESCRIPTION "bloom_filter interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_subdirectory(hasher_adapter)
add_subdirectory(hasher)