#array target definition
set(ARRAY_LIBRARY_NAME arrays)
set(ARRAY_LIBRARY_NAME ${ARRAY_LIBRARY_NAME} PARENT_SCOPE)
add_library(${ARRAY_LIBRARY_NAME} INTERFACE)

set(ARRAY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/arrays/src/cpp/)
set(ARRAY_INCLUDE_DIRS ${ARRAY_INCLUDE_DIRS} PARENT_SCOPE)
target_include_directories(${ARRAY_LIBRARY_NAME} INTERFACE ${ARRAY_INCLUDE_DIRS})
target_link_libraries(${ARRAY_LIBRARY_NAME} INTERFACE 
                                            ${UTILS_EXCEPTION_LIBRARY_NAME}
                                            )

#stack target definition
set(STACKS_LIBRARY_NAME stacks)
set(STACKS_LIBRARY_NAME ${STACKS_LIBRARY_NAME} PARENT_SCOPE)
add_library(${STACKS_LIBRARY_NAME} INTERFACE)
target_include_directories(${STACKS_LIBRARY_NAME} INTERFACE stacks/src/cpp/)
target_link_libraries(${STACKS_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})


#queue target definition
set(QUEUES_LIBRARY_NAME queues)
set(QUEUES_LIBRARY_NAME ${QUEUES_LIBRARY_NAME} PARENT_SCOPE)
add_library(${QUEUES_LIBRARY_NAME} INTERFACE)
target_include_directories(${QUEUES_LIBRARY_NAME} INTERFACE queues/src/cpp/)
target_link_libraries(${QUEUES_LIBRARY_NAME} INTERFACE ${STACKS_LIBRARY_NAME})


#graph target definition
set(GRAPHS_LIBRARY_NAME graphs)
set(GRAPHS_LIBRARY_NAME ${GRAPHS_LIBRARY_NAME} PARENT_SCOPE)
add_library(${GRAPHS_LIBRARY_NAME} INTERFACE)
target_include_directories(${GRAPHS_LIBRARY_NAME} INTERFACE graphs/src/cpp/)
target_link_libraries(${GRAPHS_LIBRARY_NAME} INTERFACE 
                                            ${ARRAY_LIBRARY_NAME}
                                            ${QUEUES_LIBRARY_NAME}
                                            )


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

file(GLOB ALL_CONFIGS config/*.json)
install(FILES ${ALL_CONFIGS} DESTINATION structures/config)

file(GLOB ARRAY_CONFIGS arrays/tests/config/*.json)
install(FILES ${ARRAY_CONFIGS} DESTINATION structures/config/arrays)

file(GLOB STACK_CONFIGS stacks/tests/config/*.json)
install(FILES ${STACK_CONFIGS} DESTINATION structures/config/stacks)

file(GLOB QUEUE_CONFIGS queues/tests/config/*.json)
install(FILES ${QUEUE_CONFIGS} DESTINATION structures/config/queues)
