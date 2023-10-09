#builder pattern target definition
set(BUILDER_PATTERN_LIBRARY_NAME builder)
set(BUILDER_PATTERN_LIBRARY_NAME ${BUILDER_PATTERN_LIBRARY_NAME} PARENT_SCOPE)
add_library(${BUILDER_PATTERN_LIBRARY_NAME} INTERFACE)

set(BUILDER_PATTERN_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/creational/builder/)
set(BUILDER_PATTERN_INCLUDE_DIRS ${BUILDER_PATTERN_INCLUDE_DIRS} PARENT_SCOPE)
target_include_directories(${BUILDER_PATTERN_LIBRARY_NAME} INTERFACE ${BUILDER_PATTERN_INCLUDE_DIRS})

#target_link_libraries(${BUILDER_PATTERN_LIBRARY_NAME} INTERFACE 
#                                            ${UTILS_EXCEPTION_LIBRARY_NAME}
#                                            )


file(GLOB BUILDER_PATTERN_HEADERS creational/builder/*.hh)

install(FILES ${BUILDER_PATTERN_HEADERS}
                DESTINATION
                patterns/builder)