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


