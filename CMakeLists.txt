cmake_minimum_required(VERSION 3.16.0)
project(utils)
include(FetchContent)

#Specify cmake options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_STATIC_LIBS "Build using static libraries" OFF)
option(CMAKE_BUILD_TYPE "Build using this build type" Debug)

#Specify the location to find files to tell cmake how to resolve third party modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/find-cmakes/")

add_subdirectory(exceptions)
add_subdirectory(parsers)
add_subdirectory(validate)
add_subdirectory(structures)
