cmake_minimum_required(VERSION 3.17)

project(hadamard-matrix)

set(SOURCE_MAIN equivalence/main.c)
set(SOURCE_LIB equivalence/equivalence.c)

add_library(equivalence STATIC ${SOURCE_LIB})
add_executable(main ${SOURCE_MAIN})

target_link_libraries(equivalence m)
target_link_libraries(main equivalence)
