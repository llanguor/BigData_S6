cmake_minimum_required(VERSION 3.31)
project(hasher_md5)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)


add_library(hasher_md5 ${HEADERS})
target_include_directories(hasher_md5 PUBLIC ./include)
target_link_libraries(hasher_md5 PUBLIC hasher)

set_target_properties(
        hasher_md5 PROPERTIES
        VERSION 1.0
        DESCRIPTION "hasher_md5 library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)