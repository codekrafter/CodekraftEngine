cmake_minimum_required(VERSION 3.5)
project(CKTestGame)

if(${CMAKE_MINOR_VERSION} GREATER 10)
cmake_policy(SET CMP0072 NEW)
endif()

set(CMAKE_INSTALL_PREFIX ..)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB SRC "src/*.cpp")

include_directories(include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../engine/src)
#link_directories()

add_executable(TestGame ${SRC})
#${CMAKE_BINARY_DIR}/engine/lib .a
#target_link_libraries(TestGame ${CMAKE_BINARY_DIR}/engine/libCKEngine.a glfw g3logger ${OPENGL_LIBRARIES} pthread dl)
target_link_libraries(TestGame ${CMAKE_BINARY_DIR}/engine/libCKEngine.so)
#if("${UNIX}" AND NOT "${APPLE}")
#target_link_libraries(TestGame X11 Xrandr Xinerama Xi Xxf86vm Xcursor)
#endif()
#if("${APPLE}")
#FIND_LIBRARY(Cocoa Cocoa)
#FIND_LIBRARY(IOKit IOKit)
#FIND_LIBRARY(CoreDisplay CoreDisplay)
#target_link_libraries(TestGame ${Cocoa} ${IOKit} ${CoreDisplay})
#endif()