cmake_minimum_required(VERSION 3.31)
project(hash_provider)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(hash_provider ${HEADERS})
target_include_directories(hash_provider PUBLIC include)

set_target_properties(
        hash_provider PROPERTIES
        VERSION 1.0
        DESCRIPTION "hash_provider interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_subdirectory(hash_provider_bytes)
add_subdirectory(hash_provider_numeric)
add_subdirectory(hash_provider_adapter)