project(CodekraftBuild)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(CodekraftBuild src/main.cpp)

set_target_properties(CodekraftBuild PROPERTIES CXX_STANDARD 17 OUTPUT_NAME ckbuild)

install(TARGETS CodekraftBuild RUNTIME DESTINATION bin)