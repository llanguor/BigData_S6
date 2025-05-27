cmake_minimum_required(VERSION 3.31)
project(BigData_S6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
include_directories("${OPENSSL_ROOT_DIR}/include")
find_library(OPENSSL_CRYPTO_LIBRARY libcrypto PATHS "${OPENSSL_ROOT_DIR}/lib/VC/x64/MDd" NO_DEFAULT_PATH)
find_library(OPENSSL_SSL_LIBRARY libssl PATHS "${OPENSSL_ROOT_DIR}/lib/VC/x64/MDd" NO_DEFAULT_PATH)
set(OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})


add_subdirectory(associative_container)
add_executable(B_Tree main_b_tree.cpp)
target_link_libraries(
        B_Tree
        PRIVATE
        associative_container
        search_tree
        b_tree)

add_subdirectory(bloom_filter)
add_executable(Bloom_Filter main_bloom_filter.cpp)
target_link_libraries(
        Bloom_Filter
        PRIVATE
        bloom_filter
        bloom_filter_builder
        hasher_md5
        hasher
        ${OPENSSL_LIBRARIES}
        "${OPENSSL_ROOT_DIR}/include")



