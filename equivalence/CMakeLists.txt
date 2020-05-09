set(SOURCE_MAIN main.c)
set(SOURCE_XOR matrix_xor.c)
set(MATRIX_LIB matrix.c)
set(SOURCE_LIB equivalence.c)

add_library(equivalence STATIC ${SOURCE_LIB})
add_library(matrix STATIC ${MATRIX_LIB})
add_executable(main ${SOURCE_MAIN})
add_executable(matrix_xor ${SOURCE_XOR})


target_link_libraries(equivalence m)
target_link_libraries(equivalence matrix)
target_link_libraries(main matrix)
target_link_libraries(main equivalence)
target_link_libraries(matrix_xor matrix)
