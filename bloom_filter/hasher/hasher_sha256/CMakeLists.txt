cmake_minimum_required(VERSION 3.31)
project(hasher_sha256)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)


add_library(hasher_sha256 ${HEADERS})
target_include_directories(hasher_sha256 PUBLIC ./include)
target_link_libraries(hasher_sha256 PUBLIC hasher)

set_target_properties(
        hasher_sha256 PROPERTIES
        VERSION 1.0
        DESCRIPTION "hasher_sha256 library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)