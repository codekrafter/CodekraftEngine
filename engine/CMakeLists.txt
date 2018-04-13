set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

cmake_minimum_required(VERSION 3.5)
project(CKEngine)

include(cotire)

ADD_DEFINITIONS(
#    -std=c++1z
#    -frtti
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(ENGINE_VERSION 0.0.1)
#set(CMAKE_INSTALL_PREFIX ..)

#find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

#set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -D_GLIBCXX_DEBUG -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE ThirdParty_SRC "include/*.c" "include/*.cpp" "include/*.inl")
file(GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "include/*.hpp" "include/*.h" "include/*.c" "include/*.inl")
foreach(hname ${HEADERS})
  string(REPLACE "/" ";" hlist ${hname})
  #list(LENGTH hlist len)
  #message(STATUS "len = ${len}")
  set(HOUT "")
  foreach(tm ${hlist})
  if(${tm} STREQUAL "include")
      set(HOUT "${HOUT}/include/CK")
    else(${tm} STREQUAL "include")
      set(HOUT "${HOUT}/${tm}")
    endif(${tm} STREQUAL "include")
  endforeach(tm ${TESTLIST})
  string(SUBSTRING ${HOUT} 0 1 PRE)
  if(${PRE} STREQUAL "/")
    string(SUBSTRING ${HOUT} 1 -1 HOUT)
  endif(${PRE} STREQUAL "/")
  configure_file(${hname} ${HOUT} COPYONLY)
endforeach(hname)
configure_file (
  "include/EngineConfig.hpp.in"
  "include/CK/EngineConfig.hpp"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/CK/)
#include_directories(/usr/local/GLFW/include)

add_library(CKEngine STATIC ${SRC} ${ThirdParty_SRC})
target_link_libraries(CKEngine glfw ${OPENGL_LIBRARIES} pthread dl)
if(${UNIX} AND NOT ${APPLE})
  target_link_libraries(CKEngine X11 Xrandr Xinerama Xi Xxf86vm Xcursor)
endif(${UNIX} AND NOT ${APPLE})
if(${APPLE})
  Find_Library(Cocoa Cocoa)
  Find_Library(IOKit IOKit)
  Find_Library(CoreDisplay CoreDisplay)
  target_link_libraries(CKEngine ${Cocoa} ${IOKit} ${CoreDisplay})
endif(${APPLE})
#cotire(CKEngine)

add_executable(CKTests test.cpp)
target_link_libraries(CKTests CKEngine)

install(TARGETS CKEngine DESTINATION lib)
install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/CK       
         DESTINATION "include")