cmake_minimum_required(VERSION 3.31)
project(hasher)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
)

add_library(hasher ${HEADERS} ${SOURCES})
target_include_directories(hasher PUBLIC ./include)

set_target_properties(
        hasher PROPERTIES
        VERSION 1.0
        DESCRIPTION "hasher interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_subdirectory(hasher_md5)