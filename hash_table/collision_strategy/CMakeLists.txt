cmake_minimum_required(VERSION 3.31)
project(collision_strategy)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

add_library(collision_strategy ${HEADERS})
target_include_directories(collision_strategy PUBLIC include)

set_target_properties(
        collision_strategy PROPERTIES
        VERSION 1.0
        DESCRIPTION "collision_strategy interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)