#search target definition
set(ARRAY_SEARCH_LIBRARY_NAME arrays_search)
set(ARRAY_SEARCH_LIBRARY_NAME ${ARRAY_SEARCH_LIBRARY_NAME} PARENT_SCOPE)
add_library(${ARRAY_SEARCH_LIBRARY_NAME} INTERFACE)

set(ARRAY_SEARCH_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/search/)
set(ARRAY_SEARCH_INCLUDE_DIRS ${ARRAY_SEARCH_INCLUDE_DIRS} PARENT_SCOPE)
target_include_directories(${ARRAY_SEARCH_LIBRARY_NAME} INTERFACE ${ARRAY_SEARCH_INCLUDE_DIRS})
target_link_libraries(${ARRAY_SEARCH_LIBRARY_NAME} INTERFACE
                                            ${ARRAY_SEARCH_LIBRARY_NAME}
                                            ${UTILS_EXCEPTION_LIBRARY_NAME}
                                            )

#graph algorithms target definition
set(GRAPH_ALGORITHMS_LIBRARY_NAME graph_algorithms)
set(GRAPH_ALGORITHMS_LIBRARY_NAME ${GRAPH_ALGORITHMS_LIBRARY_NAME} PARENT_SCOPE)
add_library(${GRAPH_ALGORITHMS_LIBRARY_NAME} INTERFACE)

set(GRAPH_ALGORITHMS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/graphs/)
set(GRAPH_ALGORITHMS_INCLUDE_DIRS ${GRAPH_ALGORITHMS_INCLUDE_DIRS} PARENT_SCOPE)
target_include_directories(${GRAPH_ALGORITHMS_LIBRARY_NAME} INTERFACE ${GRAPH_ALGORITHMS_INCLUDE_DIRS})
target_link_libraries(${GRAPH_ALGORITHMS_LIBRARY_NAME} INTERFACE
                                            ${GRAPHS_LIBRARY_NAME}
                                            ${QUEUES_LIBRARY_NAME}
                                            ${ARRAY_SEARCH_LIBRARY_NAME}
                                            ${BUILDER_PATTERN_LIBRARY_NAME}
                                            ${STRUCTURES_BUILDER_LIBRARY_NAME}
                                            ${GRAPH_BUILDER_LIBRARY_NAME}
                                            ${UTILS_EXCEPTION_LIBRARY_NAME}
                                            )


