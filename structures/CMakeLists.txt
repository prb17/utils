#array target definition
set(ARRAY_LIBRARY_NAME arrays)
set(ARRAY_TEST_NAME array_test)
add_library(${ARRAY_LIBRARY_NAME} INTERFACE)
target_include_directories(${ARRAY_LIBRARY_NAME} INTERFACE arrays/src/cpp/)
target_link_libraries(${ARRAY_LIBRARY_NAME} INTERFACE ${UTILS_EXCEPTION_LIBRARY_NAME})

add_executable(${ARRAY_TEST_NAME} arrays/tests/cpp/array_test.cc)
target_link_libraries(${ARRAY_TEST_NAME} PRIVATE ${ARRAY_LIBRARY_NAME})

#stack target definition
set(STACKS_LIBRARY_NAME stacks)
set(STACKS_TEST_NAME stacks_test)
add_library(${STACKS_LIBRARY_NAME} INTERFACE)
target_include_directories(${STACKS_LIBRARY_NAME} INTERFACE stacks/src/cpp/)
target_link_libraries(${STACKS_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})

add_executable(${STACKS_TEST_NAME} stacks/tests/cpp/stack_test.cc)
target_link_libraries(${STACKS_TEST_NAME} PRIVATE ${STACKS_LIBRARY_NAME})

#queue target definition
set(QUEUES_LIBRARY_NAME queues)
set(QUEUES_TEST_NAME queues_test)
add_library(${QUEUES_LIBRARY_NAME} INTERFACE)
target_include_directories(${QUEUES_LIBRARY_NAME} INTERFACE queues/src/cpp/)
target_link_libraries(${QUEUES_LIBRARY_NAME} INTERFACE ${STACKS_LIBRARY_NAME})

add_executable(${QUEUES_TEST_NAME} queues/tests/cpp/queue_test.cc)
target_link_libraries(${QUEUES_TEST_NAME} PRIVATE ${QUEUES_LIBRARY_NAME})

#graph target definition
set(GRAPHS_LIBRARY_NAME graphs)
set(GRAPHS_TEST_NAME graphs_test)
add_library(${GRAPHS_LIBRARY_NAME} INTERFACE)
target_include_directories(${GRAPHS_LIBRARY_NAME} INTERFACE graphs/src/cpp/)
target_link_libraries(${GRAPHS_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})

add_executable(${GRAPHS_TEST_NAME} graphs/tests/cpp/graph_test.cc)
target_link_libraries(${GRAPHS_TEST_NAME} PRIVATE ${GRAPHS_LIBRARY_NAME}) #todo: make this the list library??
