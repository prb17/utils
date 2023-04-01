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

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
set(UTILS_DEPS_BUILD_DIR "${CMAKE_BINARY_DIR}/third_party")
set(UTILS_DEPS_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/third_party")

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#Fetch and make available external projects
find_package(JsonCpp 1.9.5 COMPONENTS jsoncpp)

#TODO: figure out good options for include or not including certain utilities
add_subdirectory(exceptions)
add_subdirectory(logger)
add_subdirectory(structures)
add_subdirectory(parsers)
add_subdirectory(validator)


#####################################################################################
option(BUILD_UTILS_TESTS "Build the utils tests" OFF)
if(BUILD_UTILS_TESTS)
    # Configure library runtime path when installed
    list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${PARSERS_LIBRARY_NAME}/lib")

    # Configure array tests
    message(DEBUG "configuring the array tests")
    set(ARRAY_TEST_NAME array_test)
    set(CONTAINER_TEST_NAME container_test)
    add_executable(${ARRAY_TEST_NAME} structures/arrays/tests/cpp/array_test.cc)
    add_executable(${CONTAINER_TEST_NAME} structures/arrays/tests/cpp/container_test.cc)
    
    target_include_directories(${ARRAY_TEST_NAME} INTERFACE ${VALIDATOR_INCLUDE_DIRS})
    target_include_directories(${CONTAINER_TEST_NAME} INTERFACE ${VALIDATOR_INCLUDE_DIRS})
    target_link_libraries(${ARRAY_TEST_NAME} PRIVATE ${ARRAY_LIBRARY_NAME} ${VALIDATOR_LIBRARY_NAME} ${PARSERS_LIBRARY_NAME})
    target_link_libraries(${CONTAINER_TEST_NAME} PRIVATE ${ARRAY_LIBRARY_NAME} ${VALIDATOR_LIBRARY_NAME} ${PARSERS_LIBRARY_NAME})

    # Configure stack tests
    message(DEBUG "configuring the stack tests")
    set(STACKS_TEST_NAME stacks_test)
    add_executable(${STACKS_TEST_NAME} structures/stacks/tests/cpp/stack_test.cc)
    target_link_libraries(${STACKS_TEST_NAME} PRIVATE ${STACKS_LIBRARY_NAME})

    # Configur queues tests
    message(DEBUG "configuring the queues tests")
    set(QUEUES_TEST_NAME queues_test)
    add_executable(${QUEUES_TEST_NAME} structures/queues/tests/cpp/queue_test.cc)
    target_link_libraries(${QUEUES_TEST_NAME} PRIVATE ${QUEUES_LIBRARY_NAME})

    # Configure graphs tests
    message(DEBUG "configuring the graphs tests")
    set(GRAPHS_TEST_NAME graphs_test)
    add_executable(${GRAPHS_TEST_NAME} structures/graphs/tests/cpp/graph_test.cc)
    target_link_libraries(${GRAPHS_TEST_NAME} PRIVATE ${GRAPHS_LIBRARY_NAME})

    # Configure target installs
    message(DEBUG "configuring the tests install")
    install(TARGETS ${ARRAY_TEST_NAME} 
                    ${CONTAINER_TEST_NAME} 
                    ${STACKS_TEST_NAME} 
                    ${QUEUES_TEST_NAME} 
                    ${GRAPHS_TEST_NAME}
                    DESTINATION
                    structures/bin
                    )
endif(BUILD_UTILS_TESTS)
