#validator target definition
message(DEBUG "Creating VALIDATOR target")
set(VALIDATOR_LIBRARY_NAME validator)
set(VALIDATOR_LIBRARY_NAME ${VALIDATOR_LIBRARY_NAME} PARENT_SCOPE)
add_library(${VALIDATOR_LIBRARY_NAME} INTERFACE)

set(VALIDATOR_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/)
set(VALIDATOR_INCLUDE_DIRS ${VALIDATOR_INCLUDE_DIRS} PARENT_SCOPE)
target_include_directories(${VALIDATOR_LIBRARY_NAME} INTERFACE ${VALIDATOR_INCLUDE_DIRS} ${PARSERS_INCLUDE_DIRS})
target_link_libraries(${VALIDATOR_LIBRARY_NAME} 
                        INTERFACE 
                        ${UTILS_EXCEPTION_LIBRARY_NAME} 
                        ${PARSERS_LIBRARY_NAME} 
                        ${JsonCpp_LIBRARIES}
                        ${MAPS_LIBRARY_NAME}
                        ${ARRAY_LIBRARY_NAME}
                        ${LOGGER_LIBRARY_NAME}
                        )

message(DEBUG "Finished creating VALIDATOR target")

file(GLOB VALIDATOR_HEADERS ${VALIDATOR_INCLUDE_DIRS}/*.hh)
install(FILES ${VALIDATOR_HEADERS} DESTINATION validator/include)
