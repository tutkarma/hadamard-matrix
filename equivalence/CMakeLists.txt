find_package(Threads)

set(SOURCE_MAIN main.c)
set(SOURCE_XOR matrix_xor.c)
set(MATRIX_LIB matrix.c)
set(SOURCE_LIB equivalence.c)
set(NAIVE_LIB naive_algorithm.c)

add_library(equivalence STATIC ${SOURCE_LIB})
add_library(matrix STATIC ${MATRIX_LIB})
add_library(naive_algorithm STATIC ${NAIVE_LIB})
add_executable(main ${SOURCE_MAIN})
add_executable(matrix_xor ${SOURCE_XOR})


target_link_libraries(equivalence m)
target_link_libraries(equivalence ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(equivalence matrix)
target_link_libraries(naive_algorithm equivalence)
target_link_libraries(main matrix equivalence naive_algorithm)
target_link_libraries(matrix_xor matrix)
