cmake_minimum_required(VERSION 3.31)
project(hasher_bytes)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(hasher_bytes ${HEADERS})
target_include_directories(hasher_bytes PUBLIC ./include)
target_link_libraries(hasher_bytes PUBLIC hasher)

set_target_properties(
        hasher_bytes PROPERTIES
        VERSION 1.0
        DESCRIPTION "hasher_bytes interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_subdirectory(hasher_sha512)
add_subdirectory(hasher_sha256)
add_subdirectory(hasher_md5)