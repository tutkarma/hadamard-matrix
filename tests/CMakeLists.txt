enable_testing()

find_package(Check REQUIRED)

include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

include_directories(${CHECK_INCLUDE_DIRS})
set(LIBS ${LIBS} ${CHECK_LIBRARIES} equivalence)
include_directories(. ../equivalence)

add_executable(test_equivalence test_equivalence.c)
target_link_libraries(test_equivalence equivalence ${LIBS} pthread)

# Create testing target and redirect its output to `Testing` folder
add_test(NAME test_equivalence COMMAND test_equivalence WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_equivalence PROPERTIES TIMEOUT 300000)

include(FindValgrind)
add_custom_target(valgrind_test_equivalence
  COMMAND ${VALGRIND_PROGRAM} --leak-check=full $<TARGET_FILE:test_equivalence>
  DEPENDS test_equivalence
)
