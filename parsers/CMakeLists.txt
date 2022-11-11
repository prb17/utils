find_package(JsonCpp)
message(STATUS "json libs: ${JsonCpp_LIBRARIES}")

#parser target definition
set(PARSERS_LIBRARY_NAME parsers)
add_library(${PARSERS_LIBRARY_NAME} src/cpp/json_parser.cc)
target_include_directories(${PARSERS_LIBRARY_NAME} PRIVATE src/cpp/ ${JsonCpp_INCLUDE_DIRS})
target_link_libraries(${PARSERS_LIBRARY_NAME} PRIVATE ${UTILS_EXCEPTION_LIBRARY_NAME} ${JsonCpp_LIBRARIES})

#parser test target definition
set(PARSERS_TEST_NAME parsers_test)
add_executable(${PARSERS_TEST_NAME} test/cpp/json_parser_test.cc)
target_include_directories(${PARSERS_TEST_NAME} PRIVATE src/cpp/ ${JsonCpp_INCLUDE_DIRS})
target_link_libraries(${PARSERS_TEST_NAME} PRIVATE ${JsonCpp_LIBRARIES} ${PARSERS_LIBRARY_NAME})