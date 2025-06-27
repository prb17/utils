cmake_minimum_required(VERSION 3.16.0)
project(utils)
include(FetchContent)

#Specify cmake options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_STATIC_LIBS "Build using static libraries" OFF)
option(BUILD_OBJECT_LIBS "Build using object libraries." OFF)
option(CMAKE_BUILD_TYPE "Build using this build type" Debug)

#Specify the location to find files to tell cmake how to resolve third party modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/find-cmakes/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
set(UTILS_DEPS_BUILD_DIR "${CMAKE_BINARY_DIR}/third_party")
set(UTILS_DEPS_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/third_party")

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#Fetch and make available external projects
find_package(JsonCpp 1.9.5 COMPONENTS jsoncpp)

#TODO: figure out good options for include or not including certain utilities
add_subdirectory(patterns)
add_subdirectory(exceptions)
add_subdirectory(time)
add_subdirectory(structures)
add_subdirectory(logger)
add_subdirectory(parsers)
add_subdirectory(validator)


#####################################################################################
option(BUILD_UTILS_TESTS "Build the utils tests" OFF)
if(BUILD_UTILS_TESTS)
    # Configure library runtime path when installed
    list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${PARSERS_LIBRARY_NAME}/lib")

    message(DEBUG "configuring the array tests")
    set(ARRAY_TEST_NAME arrays_test)
    set(CONTAINER_TEST_NAME container_test)
    add_executable(${ARRAY_TEST_NAME} structures/arrays/tests/cpp/array_test.cc)
    add_executable(${CONTAINER_TEST_NAME} structures/arrays/tests/cpp/container_test.cc)
    
    target_include_directories(${ARRAY_TEST_NAME} INTERFACE ${VALIDATOR_INCLUDE_DIRS})
    target_include_directories(${CONTAINER_TEST_NAME} INTERFACE ${VALIDATOR_INCLUDE_DIRS})
    target_link_libraries(${ARRAY_TEST_NAME} PRIVATE ${ARRAY_LIBRARY_NAME} 
                                                        ${VALIDATOR_LIBRARY_NAME} 
                                                        ${PARSERS_LIBRARY_NAME} 
                                                        ${LOGGER_LIBRARY_NAME}
                                                        )

    target_link_libraries(${CONTAINER_TEST_NAME} PRIVATE ${ARRAY_LIBRARY_NAME} 
                                                            ${VALIDATOR_LIBRARY_NAME} 
                                                            ${PARSERS_LIBRARY_NAME}
                                                        )

    message(DEBUG "configuring the stack tests")
    set(STACKS_TEST_NAME stacks_test)
    add_executable(${STACKS_TEST_NAME} structures/stacks/tests/cpp/stack_test.cc)
    target_link_libraries(${STACKS_TEST_NAME} PRIVATE ${STACKS_LIBRARY_NAME} 
                                                        ${VALIDATOR_LIBRARY_NAME} 
                                                        ${PARSERS_LIBRARY_NAME} 
                                                        ${LOGGER_LIBRARY_NAME}
                                                        )

    message(DEBUG "configuring the queues tests")
    set(QUEUES_TEST_NAME queues_test)
    add_executable(${QUEUES_TEST_NAME} structures/queues/tests/cpp/queue_test.cc)
    target_link_libraries(${QUEUES_TEST_NAME} PRIVATE ${QUEUES_LIBRARY_NAME} 
                                                        ${VALIDATOR_LIBRARY_NAME} 
                                                        ${PARSERS_LIBRARY_NAME} 
                                                        ${LOGGER_LIBRARY_NAME}
                                                        )

    message(DEBUG "configuring the sets tests")
    set(SETS_TEST_NAME sets_test)
    add_executable(${SETS_TEST_NAME} structures/sets/tests/cpp/sets_test.cc)
    target_link_libraries(${SETS_TEST_NAME} PRIVATE ${SETS_LIBRARY_NAME} 
                                                        ${VALIDATOR_LIBRARY_NAME} 
                                                        ${PARSERS_LIBRARY_NAME} 
                                                        ${LOGGER_LIBRARY_NAME}
                                                        )

    message(DEBUG "configuring the maps tests")
    set(MAPS_TEST_NAME maps_test)
    add_executable(${MAPS_TEST_NAME} structures/maps/tests/cpp/maps_test.cc)
    target_link_libraries(${MAPS_TEST_NAME} PRIVATE ${MAPS_LIBRARY_NAME} 
                                                        ${VALIDATOR_LIBRARY_NAME} 
                                                        ${PARSERS_LIBRARY_NAME} 
                                                        ${LOGGER_LIBRARY_NAME}
                                                        )

    message(DEBUG "configuring the graphs tests helper libs")
    set(GRAPHS_TEST_HELPER_LIB graphs_test_helper)
    add_library(${GRAPHS_TEST_HELPER_LIB} INTERFACE)
    set(GRAPHS_TEST_HELPER_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/structures/graphs/tests/cpp/helpers/)
    file(GLOB GRAPH_HELPER_INCS ${GRAPHS_TEST_HELPER_INC_DIR}/*.hh)
    target_include_directories(${GRAPHS_TEST_HELPER_LIB} INTERFACE ${GRAPHS_TEST_HELPER_INC_DIR}
                                                            )
    target_link_libraries(${GRAPHS_TEST_HELPER_LIB} INTERFACE ${GRAPHS_LIBRARY_NAME} 
                                                        ${PARSERS_LIBRARY_NAME} 
                                                        ${BUILDER_PATTERN_LIBRARY_NAME}
                                                        ${STRUCTURES_BUILDER_LIBRARY_NAME}
                                                        ${GRAPH_BUILDER_LIBRARY_NAME}
                                                        )

    message(DEBUG "configuring the graphs tests")
    set(GRAPHS_TEST_NAME graphs_test)
    add_executable(${GRAPHS_TEST_NAME} structures/graphs/tests/cpp/graph_test.cc)
    target_link_libraries(${GRAPHS_TEST_NAME} PRIVATE ${GRAPHS_LIBRARY_NAME} 
                                                        ${LINKED_LISTS_LIBRARY_NAME}
                                                        ${VALIDATOR_LIBRARY_NAME} 
                                                        ${PARSERS_LIBRARY_NAME} 
                                                        ${LOGGER_LIBRARY_NAME}
                                                        ${BUILDER_PATTERN_LIBRARY_NAME}
                                                        ${STRUCTURES_BUILDER_LIBRARY_NAME}
                                                        ${GRAPH_BUILDER_LIBRARY_NAME}
                                                        ${GRAPHS_TEST_HELPER_LIB}
                                                        )

    message(DEBUG "configuring the linked list tests")
    set(LINKED_LIST_TEST_NAME linked_list_test)
    add_executable(${LINKED_LIST_TEST_NAME} structures/graphs/tests/cpp/linked_list_test.cc)
    target_link_libraries(${LINKED_LIST_TEST_NAME} PRIVATE ${GRAPHS_LIBRARY_NAME} 
                                                            ${VALIDATOR_LIBRARY_NAME} 
                                                            ${PARSERS_LIBRARY_NAME} 
                                                            ${LOGGER_LIBRARY_NAME}
                                                            ${BUILDER_PATTERN_LIBRARY_NAME}
                                                            ${STRUCTURES_BUILDER_LIBRARY_NAME}
                                                            ${GRAPH_BUILDER_LIBRARY_NAME}
                                                            )
    
    message(DEBUG "configuring the tree tests")
    set(TREE_TEST_NAME tree_test)
    add_executable(${TREE_TEST_NAME} structures/graphs/tests/cpp/tree_test.cc)
    target_link_libraries(${TREE_TEST_NAME} PRIVATE ${GRAPHS_LIBRARY_NAME} 
                                                        ${VALIDATOR_LIBRARY_NAME} 
                                                        ${PARSERS_LIBRARY_NAME} 
                                                        ${LOGGER_LIBRARY_NAME}
                                                        ${BUILDER_PATTERN_LIBRARY_NAME}
                                                        ${STRUCTURES_BUILDER_LIBRARY_NAME}
                                                        ${GRAPH_BUILDER_LIBRARY_NAME}
                                                        ${GRAPHS_TEST_HELPER_LIB}
                                                        )

    message(DEBUG "configuring the tests install")
    install(FILES ${GRAPH_HELPER_INCS} DESTINATION structures/include)
    install(TARGETS ${ARRAY_TEST_NAME} 
                    ${CONTAINER_TEST_NAME} 
                    ${STACKS_TEST_NAME} 
                    ${QUEUES_TEST_NAME} 
                    ${SETS_TEST_NAME} 
                    ${MAPS_TEST_NAME} 
                    ${GRAPHS_TEST_NAME}
                    ${LINKED_LIST_TEST_NAME}
                    ${TREE_TEST_NAME}
                    DESTINATION
                    structures/bin
                    )
endif(BUILD_UTILS_TESTS)
