#parser target definition
set(PARSERS_LIBRARY_NAME parsers)
set(PARSERS_LIBRARY_NAME ${PARSERS_LIBRARY_NAME} PARENT_SCOPE)

# Set runtime path of json_parser
if(BUILD_UTILS_TESTS)
    list(APPEND CMAKE_INSTALL_RPATH "${UTILS_DEPS_INSTALL_DIR}/${JsonCpp_PROJ_NAME}/lib")
endif(BUILD_UTILS_TESTS)

add_library(${PARSERS_LIBRARY_NAME} src/cpp/json_parser.cc)
add_dependencies(${PARSERS_LIBRARY_NAME} ${JsonCpp_EXTERNAL_LIB_NAME})

set(PARSERS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp ${JsonCpp_INCLUDE_DIRS})
set(PARSERS_INCLUDE_DIRS ${PARSERS_INCLUDE_DIRS} PARENT_SCOPE)

target_include_directories(${PARSERS_LIBRARY_NAME} PRIVATE ${PARSERS_INCLUDE_DIRS})
target_link_directories(${PARSERS_LIBRARY_NAME} PRIVATE ${JsonCpp_LIBRARIES_DIR})
target_link_libraries(${PARSERS_LIBRARY_NAME} PRIVATE ${UTILS_EXCEPTION_LIBRARY_NAME} ${JsonCpp_LIBRARIES} ${ARRAY_LIBRARY_NAME})

if(BUILD_UTILS_TESTS)
    # Remove runtime path jsoncpp before creating test binary
    unset(CMAKE_INSTALL_RPATH)

    #parser test target definition
    set(PARSERS_TEST_NAME parsers_test)
    add_executable(${PARSERS_TEST_NAME} test/cpp/json_parser_test.cc)
    add_dependencies(${PARSERS_LIBRARY_NAME} ${JsonCpp_EXTERNAL_LIB_NAME})

    target_include_directories(${PARSERS_TEST_NAME} PRIVATE ${PARSERS_INCLUDE_DIRS} ${ARRAY_INCLUDE_DIRS} ${UTILS_EXCEPTION_INCLUDE_DIRS} )

    target_link_directories(${PARSERS_TEST_NAME} PRIVATE  ${JsonCpp_LIBRARIES_DIR})
    target_link_libraries(${PARSERS_TEST_NAME} PRIVATE ${PARSERS_LIBRARY_NAME} ${JsonCpp_LIBRARIES} ${ARRAY_LIBRARY_NAME})

    install(TARGETS ${PARSERS_TEST_NAME} DESTINATION ${PARSERS_LIBRARY_NAME}/bin)
    file(GLOB PARSERS_TEST_CONFIG_FILES test/config/*.json)
    install(FILES ${PARSERS_TEST_CONFIG_FILES} DESTINATION ${PARSERS_LIBRARY_NAME}/config)
endif(BUILD_UTILS_TESTS)

install(TARGETS ${PARSERS_LIBRARY_NAME} DESTINATION ${PARSERS_LIBRARY_NAME}/lib)
file(GLOB PARSERS_HEADERS src/cpp/*.hh)
install(FILES ${PARSERS_HEADERS} DESTINATION ${PARSERS_LIBRARY_NAME}/include)
