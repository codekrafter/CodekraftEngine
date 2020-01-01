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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -DEDITOR")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")# "src/*.inl")
file(GLOB_RECURSE SRC_FRONTEND_BAD "src/Frontend/*.cpp" "src/Frontend/*.c")
file(GLOB_RECURSE SRC_FRONTEND_GOOD "src/Frontend/CurrentFrontend.cpp")

list(REMOVE_ITEM SRC ${SRC_FRONTEND_BAD})
list(APPEND SRC ${SRC_FRONTEND_GOOD})

file(GLOB_RECURSE GEN_SRC "generated/code/*.cpp")
#file(GLOB_RECURSE ThirdParty_SRC "include/*.c" "include/*.cpp" "include/*.inl")
file(GLOB_RECURSE HEADERS_INCLUDE RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/include" "include/*.hpp" "include/*.h")
file(GLOB_RECURSE HEADERS_SRC RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src" "src/*.hpp" "src/*.h" "src/*.inl")
file(GLOB_RECURSE HEADERS_GEN RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/generated/code/" "generated/code/*.hpp" "generated/code/*.h")
set(HEADERS ${HEADERS_INCLUDE} ${HEADERS_SRC})

set(${CMAKE_INSTALL_PREFIX} /opt/CodekraftEngine/)

configure_file (
  "src/EngineConfig.hpp.in"
  "include/EngineConfig.hpp"
)

foreach(HEADER ${HEADERS_INCLUDE})
configure_file(include/${HEADER} CK/${HEADER} COPYONLY)
endforeach(HEADER ${HEADERS_INCLUDE})

foreach(HEADER ${HEADERS_SRC})
configure_file(src/${HEADER} CK/${HEADER} COPYONLY)
endforeach(HEADER ${HEADERS_SRC})

foreach(HEADER ${HEADERS_GEN})
configure_file(generated/code/${HEADER} CK/${HEADER} COPYONLY)
endforeach(HEADER ${HEADERS_GEN})

#include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/CK/)
#include_directories(/usr/local/GLFW/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/generated/code/)
link_directories()

add_library(CodekraftEngine SHARED ${SRC} ${GEN_SRC})
target_link_libraries(CodekraftEngine glfw g3logger ${OPENGL_LIBRARIES} pthread dl stdc++fs)
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

install(TARGETS CodekraftEngine DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CK      
         DESTINATION "include/")


file(GLOB_RECURSE HEADERS_CONFIG RELATIVE "${CMAKE_CURRENT_BINARY_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include/*.hpp" "${CMAKE_CURRENT_BINARY_DIR}/include/*.h")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/${HEADERS_CONFIG}      
         DESTINATION "include/CK/")
