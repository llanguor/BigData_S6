cmake_minimum_required(VERSION 3.31)
project(hasher_numeric)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(hasher_numeric ${HEADERS})
target_include_directories(hasher_numeric PUBLIC ./include)
target_link_libraries(hasher_numeric PUBLIC hasher)

set_target_properties(
        hasher_numeric PROPERTIES
        VERSION 1.0
        DESCRIPTION "hasher_numeric interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)