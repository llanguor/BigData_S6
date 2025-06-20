cmake_minimum_required(VERSION 3.31)
project(hash_table)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(hash_table ${HEADERS})
target_include_directories(hash_table PUBLIC ./include)
target_link_libraries(hash_table
        PUBLIC
        ${OPENSSL_LIBRARIES}
        "${OPENSSL_ROOT_DIR}/include"
        associative_container
        hash_provider
        hash_provider_numeric
        hash_provider_adapter
        hash_provider_bytes
        hash_provider_md5
        hash_provider_sha256
        hash_provider_sha512
)

set_target_properties(
        hash_table PROPERTIES
        VERSION 1.0
        DESCRIPTION "hash_table interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)