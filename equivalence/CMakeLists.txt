set(SOURCE_MAIN main.c)
set(SOURCE_LIB equivalence.c)

add_library(equivalence STATIC ${SOURCE_LIB})
add_executable(main ${SOURCE_MAIN})

target_link_libraries(equivalence m)
target_link_libraries(main equivalence)
