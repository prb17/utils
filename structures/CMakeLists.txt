#structure builder definition                                       
set(STRUCTURES_BUILDER_LIBRARY_NAME structures_builder)
set(STRUCTURES_BUILDER_LIBRARY_NAME ${STRUCTURES_BUILDER_LIBRARY_NAME} PARENT_SCOPE)
add_library(${STRUCTURES_BUILDER_LIBRARY_NAME} INTERFACE)
target_include_directories(${STRUCTURES_BUILDER_LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/builder/src/cpp/)

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


#sets target definition
set(SETS_LIBRARY_NAME sets)
set(SETS_LIBRARY_NAME ${SETS_LIBRARY_NAME} PARENT_SCOPE)
add_library(${SETS_LIBRARY_NAME} INTERFACE)
target_include_directories(${SETS_LIBRARY_NAME} INTERFACE sets/src/cpp/)
target_link_libraries(${SETS_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})


#maps target definition
set(MAPS_LIBRARY_NAME maps)
set(MAPS_LIBRARY_NAME ${MAPS_LIBRARY_NAME} PARENT_SCOPE)
add_library(${MAPS_LIBRARY_NAME} INTERFACE)
target_include_directories(${MAPS_LIBRARY_NAME} INTERFACE maps/src/cpp/)
target_link_libraries(${MAPS_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})


#graph target definition
set(GRAPHS_LIBRARY_NAME graphs)
set(GRAPHS_LIBRARY_NAME ${GRAPHS_LIBRARY_NAME} PARENT_SCOPE)
add_library(${GRAPHS_LIBRARY_NAME} INTERFACE)
target_include_directories(${GRAPHS_LIBRARY_NAME} INTERFACE graphs/src/cpp/)
target_link_libraries(${GRAPHS_LIBRARY_NAME} INTERFACE 
                                            ${ARRAY_LIBRARY_NAME}
                                            ${QUEUES_LIBRARY_NAME}
                                            ${GRAPH_BUILDER_LIBRARY_NAME}
                                            ${STRUCTURES_BUILDER_LIBRARY_NAME}
                                            )

# graph builder target definition
set(GRAPH_BUILDER_LIBRARY_NAME graph_builder)
set(GRAPH_BUILDER_LIBRARY_NAME ${GRAPH_BUILDER_LIBRARY_NAME} PARENT_SCOPE)
add_library(${GRAPH_BUILDER_LIBRARY_NAME} INTERFACE)
target_include_directories(${GRAPH_BUILDER_LIBRARY_NAME} INTERFACE graphs/src/cpp/builder/)
target_link_libraries(${STRUCTURES_BUILDER_LIBRARY_NAME} INTERFACE
                                            ${GRAPHS_LIBRARY_NAME}
                                            )

#trees target definition (a tree is a specialization of a graph)
set(TREES_LIBRARY_NAME trees)
set(TREES_LIBRARY_NAME ${TREES_LIBRARY_NAME} PARENT_SCOPE)
add_library(${TREES_LIBRARY_NAME} INTERFACE)
target_include_directories(${TREES_LIBRARY_NAME} INTERFACE graphs/src/cpp/trees/)
target_link_libraries(${TREES_LIBRARY_NAME} INTERFACE
                                            ${GRAPHS_LIBRARY_NAME}
                                            )

#heap target definition (complete binary tree encoded in an array)
set(HEAPS_LIBRARY_NAME heaps)
set(HEAPS_LIBRARY_NAME ${HEAPS_LIBRARY_NAME} PARENT_SCOPE)
add_library(${HEAPS_LIBRARY_NAME} INTERFACE)
target_include_directories(${HEAPS_LIBRARY_NAME} INTERFACE heaps/src/cpp/)
target_link_libraries(${HEAPS_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})

#priority queue target definition (adaptor over heap)
set(PRIORITY_QUEUES_LIBRARY_NAME priority_queues)
set(PRIORITY_QUEUES_LIBRARY_NAME ${PRIORITY_QUEUES_LIBRARY_NAME} PARENT_SCOPE)
add_library(${PRIORITY_QUEUES_LIBRARY_NAME} INTERFACE)
target_include_directories(${PRIORITY_QUEUES_LIBRARY_NAME} INTERFACE priority_queues/src/cpp/)
target_link_libraries(${PRIORITY_QUEUES_LIBRARY_NAME} INTERFACE ${HEAPS_LIBRARY_NAME})

#sorted sequence target definition (array with a maintained sort order)
set(SORTED_SEQUENCES_LIBRARY_NAME sorted_sequences)
set(SORTED_SEQUENCES_LIBRARY_NAME ${SORTED_SEQUENCES_LIBRARY_NAME} PARENT_SCOPE)
add_library(${SORTED_SEQUENCES_LIBRARY_NAME} INTERFACE)
target_include_directories(${SORTED_SEQUENCES_LIBRARY_NAME} INTERFACE sorted_sequences/src/cpp/)
target_link_libraries(${SORTED_SEQUENCES_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})

#balanced bst target definition (self-balancing AVL tree)
set(BST_LIBRARY_NAME bst)
set(BST_LIBRARY_NAME ${BST_LIBRARY_NAME} PARENT_SCOPE)
add_library(${BST_LIBRARY_NAME} INTERFACE)
target_include_directories(${BST_LIBRARY_NAME} INTERFACE bst/src/cpp/)
target_link_libraries(${BST_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})

#ring buffer target definition (fixed-capacity circular buffer)
set(RING_BUFFERS_LIBRARY_NAME ring_buffers)
set(RING_BUFFERS_LIBRARY_NAME ${RING_BUFFERS_LIBRARY_NAME} PARENT_SCOPE)
add_library(${RING_BUFFERS_LIBRARY_NAME} INTERFACE)
target_include_directories(${RING_BUFFERS_LIBRARY_NAME} INTERFACE ring_buffers/src/cpp/)
target_link_libraries(${RING_BUFFERS_LIBRARY_NAME} INTERFACE ${ARRAY_LIBRARY_NAME})

file(GLOB STRUCTURES_BUILDER_HEADERS buider/src/cpp/*.hh)
file(GLOB ARRAY_HEADERS arrays/src/cpp/*.hh)
file(GLOB STACKS_HEADERS stacks/src/cpp/*.hh)
file(GLOB QUEUES_HEADERS queues/src/cpp/*.hh)
file(GLOB SETS_HEADERS sets/src/cpp/*.hh)
file(GLOB MAPS_HEADERS maps/src/cpp/*.hh)
file(GLOB GRAPHS_HEADERS graphs/src/cpp/*.hh)
file(GLOB GRAPH_BUILDER_HEADERS graphs/builder/cpp/*.hh)
file(GLOB LINKED_LISTS_HEADERS graphs/src/cpp/linked_lists/*.hh)
file(GLOB TREES_HEADERS graphs/src/cpp/trees/*.hh)
file(GLOB HEAPS_HEADERS heaps/src/cpp/*.hh)
file(GLOB PRIORITY_QUEUES_HEADERS priority_queues/src/cpp/*.hh)
file(GLOB SORTED_SEQUENCES_HEADERS sorted_sequences/src/cpp/*.hh)
file(GLOB BST_HEADERS bst/src/cpp/*.hh)
file(GLOB RING_BUFFERS_HEADERS ring_buffers/src/cpp/*.hh)

install(FILES ${STRUCTURES_BUILDER_HEADERS}
                ${ARRAY_HEADERS}
                ${STACKS_HEADERS}
                ${QUEUES_HEADERS}
                ${SETS_HEADERS}
                ${MAPS_HEADERS}
                ${GRAPHS_HEADERS}
                ${GRAPH_BUILDER_HEADERS}
                ${TREES_HEADERS}
                ${HEAPS_HEADERS}
                ${PRIORITY_QUEUES_HEADERS}
                ${SORTED_SEQUENCES_HEADERS}
                ${BST_HEADERS}
                ${RING_BUFFERS_HEADERS}
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

file(GLOB SET_CONFIGS sets/tests/config/*.json)
install(FILES ${SET_CONFIGS} DESTINATION structures/config/sets)

file(GLOB MAP_CONFIGS maps/tests/config/*.json)
install(FILES ${MAP_CONFIGS} DESTINATION structures/config/maps)

file(GLOB GRAPH_CONFIGS graphs/tests/config/*.json)
install(FILES ${GRAPH_CONFIGS} DESTINATION structures/config/graphs)

file(GLOB LINKED_LISTS_CONFIGS graphs/tests/config/linked_lists/*.json)
install(FILES ${LINKED_LISTS_CONFIGS} DESTINATION structures/config/graphs/linked_lists)

file(GLOB TREES_CONFIGS graphs/tests/config/trees/*.json)
install(FILES ${TREES_CONFIGS} DESTINATION structures/config/graphs/trees)

file(GLOB HEAP_CONFIGS heaps/tests/config/*.json)
install(FILES ${HEAP_CONFIGS} DESTINATION structures/config/heaps)

file(GLOB PRIORITY_QUEUE_CONFIGS priority_queues/tests/config/*.json)
install(FILES ${PRIORITY_QUEUE_CONFIGS} DESTINATION structures/config/priority_queues)

file(GLOB SORTED_SEQUENCE_CONFIGS sorted_sequences/tests/config/*.json)
install(FILES ${SORTED_SEQUENCE_CONFIGS} DESTINATION structures/config/sorted_sequences)

file(GLOB BST_CONFIGS bst/tests/config/*.json)
install(FILES ${BST_CONFIGS} DESTINATION structures/config/bst)

file(GLOB RING_BUFFER_CONFIGS ring_buffers/tests/config/*.json)
install(FILES ${RING_BUFFER_CONFIGS} DESTINATION structures/config/ring_buffers)
