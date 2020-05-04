cmake_minimum_required(VERSION 3.17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(hadamard-matrix)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_subdirectory(equivalence)

enable_testing()

add_subdirectory(tests)
