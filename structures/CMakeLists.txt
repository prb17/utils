#array target definition
set(ARRAY_LIBRARY_NAME arrays)
set(ARRAY_TEST_NAME array_test)
add_library(${ARRAY_LIBRARY_NAME} INTERFACE)
target_include_directories(${ARRAY_LIBRARY_NAME} INTERFACE arrays/src/cpp/)

add_executable(${ARRAY_TEST_NAME} arrays/tests/cpp/array_test.cc)
target_link_libraries(${ARRAY_TEST_NAME} PRIVATE ${ARRAY_LIBRARY_NAME})

#stack target definition
set(STACKS_LIBRARY_NAME stacks)
set(STACKS_TEST_NAME stacks_test)
add_library(${STACKS_LIBRARY_NAME} INTERFACE)
target_include_directories(${STACKS_LIBRARY_NAME} INTERFACE stacks/src/cpp/)

add_executable(${STACKS_TEST_NAME} stacks/tests/cpp/stack_test.cc)
target_link_libraries(${STACKS_TEST_NAME} PRIVATE ${STACKS_LIBRARY_NAME} ${ARRAY_LIBRARY_NAME})
