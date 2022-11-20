#array target definition
set(ARRAY_LIBRARY_NAME arrays)
add_library(${ARRAY_LIBRARY_NAME} INTERFACE)
target_include_directories(${ARRAY_LIBRARY_NAME} INTERFACE arrays/src/cpp/)
target_link_libraries(${ARRAY_LIBRARY_NAME} INTERFACE ${UTILS_EXCEPTION_LIBRARY_NAME})

if(BUILD_UTILS_TESTS)
    set(ARRAY_TEST_NAME array_test)
    set(CONTAINER_TEST_NAME container_test)
    add_executable(${ARRAY_TEST_NAME} arrays/tests/cpp/array_test.cc)
    add_executable(${CONTAINER_TEST_NAME} arrays/tests/cpp/container_test.cc)
    target_link_libraries(${ARRAY_TEST_NAME} PRIVATE ${ARRAY_LIBRARY_NAME})
    target_link_libraries(${CONTAINER_TEST_NAME} PRIVATE ${ARRAY_LIBRARY_NAME})
endif(BUILD_UTILS_TESTS)

#stack target definition
set(STACKS_LIBRARY_NAME stacks)
add_library(${STACKS_LIBRARY_NAME} INTERFACE)
target_include_directories(${STACKS_LIBRARY_NAME} INTERFACE stacks/src/cpp/)
target_link_libraries(${STACKS_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})

if(BUILD_UTILS_TESTS)
    set(STACKS_TEST_NAME stacks_test)
    add_executable(${STACKS_TEST_NAME} stacks/tests/cpp/stack_test.cc)
    target_link_libraries(${STACKS_TEST_NAME} PRIVATE ${STACKS_LIBRARY_NAME})
endif(BUILD_UTILS_TESTS)

#queue target definition
set(QUEUES_LIBRARY_NAME queues)
add_library(${QUEUES_LIBRARY_NAME} INTERFACE)
target_include_directories(${QUEUES_LIBRARY_NAME} INTERFACE queues/src/cpp/)
target_link_libraries(${QUEUES_LIBRARY_NAME} INTERFACE ${STACKS_LIBRARY_NAME})

if(BUILD_UTILS_TESTS)
    set(QUEUES_TEST_NAME queues_test)
    add_executable(${QUEUES_TEST_NAME} queues/tests/cpp/queue_test.cc)
    target_link_libraries(${QUEUES_TEST_NAME} PRIVATE ${QUEUES_LIBRARY_NAME})
endif(BUILD_UTILS_TESTS)

#graph target definition
set(GRAPHS_LIBRARY_NAME graphs)
add_library(${GRAPHS_LIBRARY_NAME} INTERFACE)
target_include_directories(${GRAPHS_LIBRARY_NAME} INTERFACE graphs/src/cpp/)
target_link_libraries(${GRAPHS_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})

if(BUILD_UTILS_TESTS)
    set(GRAPHS_TEST_NAME graphs_test)
    add_executable(${GRAPHS_TEST_NAME} graphs/tests/cpp/graph_test.cc)
    target_link_libraries(${GRAPHS_TEST_NAME} PRIVATE ${GRAPHS_LIBRARY_NAME})
endif(BUILD_UTILS_TESTS)

if(BUILD_UTILS_TESTS)
    install(TARGETS ${ARRAY_TEST_NAME} 
                    ${CONTAINER_TEST_NAME} 
                    ${STACKS_TEST_NAME} 
                    ${QUEUES_TEST_NAME} 
                    ${GRAPHS_TEST_NAME}
                    DESTINATION
                    structures/bin
                    )
endif(BUILD_UTILS_TESTS)

file(GLOB ARRAY_HEADERS arrays/src/cpp/*.hh)
file(GLOB STACKS_HEADERS stacks/src/cpp/*.hh)
file(GLOB QUEUES_HEADERS queues/src/cpp/*.hh)
file(GLOB GRAPHS_HEADERS graphs/src/cpp/*.hh)

install(FILES ${ARRAY_HEADERS}
                ${STACKS_HEADERS}
                ${QUEUES_HEADERS}
                ${GRAPHS_HEADERS}
                DESTINATION
                structures/include)
