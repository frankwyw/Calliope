cmake_minimum_required (VERSION 2.6)
project(Honoka)
#set(CMAKE_CXX_COMPILER "clang++")

set(PROJECT_SOURCE_DIR "./src")
set(EXAMPLE_SOURCE_DIR "./example")
set(INCLUDE_DIR "./include")

set( CMAKE_BUILD_TYPE Debug )

set(CMAKE_LINK_LIBRARY_FLAG "${CMAKE_LINK_LIBRARY_FLAG}pthread -l ")
aux_source_directory(${PROJECT_SOURCE_DIR} CPPS)

add_subdirectory("${PROJECT_SOURCE_DIR}")
include_directories("${INCLUDE_DIR}")
ADD_DEFINITIONS("-std=c++11  -Wall  -Wextra")
add_executable(echoServer ${EXAMPLE_SOURCE_DIR}/echo.cpp ${CPPS})
add_library(Calliope STATIC ${CPPS})
target_link_libraries(echoServer glog)

