set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

cmake_minimum_required(VERSION 3.5)
project(CodekraftEngine)

if(${CMAKE_MINOR_VERSION} GREATER 10)
cmake_policy(SET CMP0072 NEW)
endif()

ADD_DEFINITIONS(
#    -std=c++1z
#    -frtti
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(ENGINE_VERSION 0.0.1)
#set(CMAKE_INSTALL_PREFIX ..)
add_subdirectory(ThirdParty/)
#find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

if(NOT DEFINED ENV{WITH_CKB})
  message(FATAL_ERROR "Please do not run cmake manually, please use the Codekraft Build Tool")
endif(NOT DEFINED ENV{WITH_CKB})

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -w -ferror-limit=2 -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")# "src/*.inl")
file(GLOB_RECURSE GEN_SRC "generated/code/*.cpp")
#file(GLOB_RECURSE ThirdParty_SRC "include/*.c" "include/*.cpp" "include/*.inl")
#file(GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "include/*.hpp" "include/*.h" "include/*.c" "include/*.inl")

configure_file (
  "src/EngineConfig.hpp.in"
  "include/EngineConfig.hpp"
)

#include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/CK/)
#include_directories(/usr/local/GLFW/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/generated/code/)
link_directories()

add_library(CodekraftEngine SHARED ${SRC} ${GEN_SRC})
target_link_libraries(CodekraftEngine glfw g3logger ${OPENGL_LIBRARIES} pthread dl)
if("${UNIX}" AND NOT "${APPLE}")
  target_link_libraries(CodekraftEngine X11 Xrandr Xinerama Xi Xxf86vm Xcursor)
endif()
if("${APPLE}")
  Find_Library(Cocoa Cocoa)
  Find_Library(IOKit IOKit)
  Find_Library(CoreDisplay CoreDisplay)
  target_link_libraries(CodekraftEngine ${Cocoa} ${IOKit} ${CoreDisplay})
endif()

add_executable(CKTests test.cpp)
target_link_libraries(CKTests CodekraftEngine)

#install(TARGETS CodekraftEngine DESTINATION lib)
#install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/CK       
#         DESTINATION "include")