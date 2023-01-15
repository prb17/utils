#parser target definition
set(PARSERS_LIBRARY_NAME parsers)
set(PARSERS_LIBRARY_NAME ${PARSERS_LIBRARY_NAME} PARENT_SCOPE)
add_library(${PARSERS_LIBRARY_NAME} src/cpp/json_parser.cc)
# if (NOT JsonCpp_EXTERNAL_LIB_NAME)
#     set(JsonCpp_EXTERNAL_LIB_NAME jsoncpp_lib)
# endif(NOT JsonCpp_EXTERNAL_LIB_NAME)
add_dependencies(${PARSERS_LIBRARY_NAME} ${JsonCpp_EXTERNAL_LIB_NAME})

set(PARSERS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp ${JsonCpp_INCLUDE_DIRS})
set(PARSERS_INCLUDE_DIRS ${PARSERS_INCLUDE_DIRS} PARENT_SCOPE)

target_include_directories(${PARSERS_LIBRARY_NAME} PRIVATE ${PARSERS_INCLUDE_DIRS})
target_link_directories(${PARSERS_LIBRARY_NAME} PRIVATE ${JsonCpp_LIBRARIES_DIR})
target_link_libraries(${PARSERS_LIBRARY_NAME} PRIVATE ${UTILS_EXCEPTION_LIBRARY_NAME} ${JsonCpp_LIBRARIES})

if(BUILD_UTILS_TESTS)
    #parser test target definition
    set(PARSERS_TEST_NAME parsers_test)
    add_executable(${PARSERS_TEST_NAME} test/cpp/json_parser_test.cc)
    add_dependencies(${PARSERS_LIBRARY_NAME} ${JsonCpp_EXTERNAL_LIB_NAME})
    # if (JsonCpp_EXTERNAL_LIB_NAME)
    #     add_dependencies(${PARSERS_TEST_NAME} ${JsonCpp_EXTERNAL_LIB_NAME})
    # else (JsonCpp_EXTERNAL_LIB_NAME)
    #     add_dependencies(${PARSERS_TEST_NAME} JsonCpp::JsonCpp)  
    # endif(JsonCpp_EXTERNAL_LIB_NAME)

    target_include_directories(${PARSERS_TEST_NAME} PRIVATE ${PARSERS_INCLUDE_DIRS})

    target_link_directories(${PARSERS_TEST_NAME} PRIVATE  ${JsonCpp_LIBRARIES_DIR})
    target_link_libraries(${PARSERS_TEST_NAME} PRIVATE ${PARSERS_LIBRARY_NAME} ${JsonCpp_LIBRARIES})

    install(TARGETS ${PARSERS_TEST_NAME} DESTINATION parsers/bin)
    file(GLOB PARSERS_TEST_CONFIG_FILES test/config/*.json)
    install(FILES ${PARSERS_TEST_CONFIG_FILES} DESTINATION parsers/config)
endif(BUILD_UTILS_TESTS)

install(TARGETS ${PARSERS_LIBRARY_NAME} DESTINATION parsers/lib)
file(GLOB PARSERS_HEADERS src/cpp/*.hh)
install(FILES ${PARSERS_HEADERS} DESTINATION parsers/include)
message(DEBUG "parsers library name: ${PARSERS_LIBRARY_NAME}")
