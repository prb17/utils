#sockets target definition (POSIX TCP sockets)
find_package(Threads REQUIRED)

set(SOCKETS_LIBRARY_NAME sockets)
set(SOCKETS_LIBRARY_NAME ${SOCKETS_LIBRARY_NAME} PARENT_SCOPE)

# static so the socket code links directly into consumers/tests and needs no
# runtime .so path of its own (the test then only needs libparsers.so, like the
# other test binaries)
add_library(${SOCKETS_LIBRARY_NAME} STATIC src/cpp/socket.cc)
set_target_properties(${SOCKETS_LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

set(SOCKETS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp)
set(SOCKETS_INCLUDE_DIRS ${SOCKETS_INCLUDE_DIRS} PARENT_SCOPE)
target_include_directories(${SOCKETS_LIBRARY_NAME} PUBLIC ${SOCKETS_INCLUDE_DIRS})
target_link_libraries(${SOCKETS_LIBRARY_NAME} PUBLIC Threads::Threads)

install(TARGETS ${SOCKETS_LIBRARY_NAME} DESTINATION ${SOCKETS_LIBRARY_NAME}/lib)
file(GLOB SOCKETS_HEADERS src/cpp/*.hh)
install(FILES ${SOCKETS_HEADERS} DESTINATION ${SOCKETS_LIBRARY_NAME}/include)

if(BUILD_UTILS_TESTS)
    #sockets test target definition
    set(SOCKETS_TEST_NAME sockets_test)
    add_executable(${SOCKETS_TEST_NAME} tests/cpp/socket_test.cc)
    target_include_directories(${SOCKETS_TEST_NAME} PRIVATE
                                            ${SOCKETS_INCLUDE_DIRS}
                                            ${VALIDATOR_INCLUDE_DIRS}
                                            ${PARSERS_INCLUDE_DIRS}
                                            ${ARRAY_INCLUDE_DIRS})
    target_link_libraries(${SOCKETS_TEST_NAME} PRIVATE
                                            ${SOCKETS_LIBRARY_NAME}
                                            ${VALIDATOR_LIBRARY_NAME}
                                            ${PARSERS_LIBRARY_NAME}
                                            ${LOGGER_LIBRARY_NAME}
                                            ${ARRAY_LIBRARY_NAME}
                                            Threads::Threads)

    # install alongside the structure tests so the CI test runner (which runs
    # from structures/bin) picks it up
    install(TARGETS ${SOCKETS_TEST_NAME} DESTINATION structures/bin)
    file(GLOB SOCKETS_TEST_CONFIGS tests/config/*.json)
    install(FILES ${SOCKETS_TEST_CONFIGS} DESTINATION structures/config/sockets)
endif(BUILD_UTILS_TESTS)
