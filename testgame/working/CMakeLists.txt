
cmake_minimum_required(VERSION 3.5)
project(TestGame)

if(${CMAKE_MINOR_VERSION} GREATER 10)
cmake_policy(SET CMP0072 NEW)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(NAME TestGame)

#NO CUSTOM FIND DIRECTIVES

set(CMAKE_BUILD_TYPE Debug)

file(GLOB SRC ".././src//*.cpp")

#NO PRECOMPILE STEPS

include_directories(/opt/CodekraftEngine/include/)
link_directories(/opt/CodekraftEngine/lib)

add_executable(TestGame ${SRC})
#target_link_libraries(TestGame ${CMAKE_BINARY_DIR}/engine/libCKEngine.so)
target_link_libraries(TestGame CodekraftEngine)
