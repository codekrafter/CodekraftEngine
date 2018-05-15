project(CodekraftBuild)
cmake_minimum_required(VERSION 3.5)

# Add ThirdParty directory
add_subdirectory(ThirdParty/)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c" "src/*.inl")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(CodekraftBuild ${SRC})