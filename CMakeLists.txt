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
find_package(JsonCpp 1.9.5 COMPONENTS jsoncpp_lib)
message(DEBUG "json incs: ${JsonCpp_INCLUDE_DIRS}")
message(DEBUG "json libs: ${JsonCpp_LIBRARIES}")
message(DEBUG "json libs dir: ${JsonCpp_LIBRARIES_DIR}")

option(BUILD_UTILS_TESTS "Build the utils tests" OFF)
add_subdirectory(exceptions)

#TODO: figure out good options for include or not including certain utilities
add_subdirectory(parsers)
add_subdirectory(validate)
add_subdirectory(structures)

