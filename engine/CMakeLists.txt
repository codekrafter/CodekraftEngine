cmake_minimum_required(VERSION 3.5)
project(CKEngine)

ADD_DEFINITIONS(
    -std=c++17 # Or -std=c++0x
    # Other flags
)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(ENGINE_VERSION 0.0.1)
set(CMAKE_INSTALL_PREFIX ..)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE TP_SRC "include/*.c" "include/*.cpp" "include/*.inl")
file(GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "include/*.hpp" "include/*.h" "include/*.c" "include/*.inl")
foreach(hname ${HEADERS})
    configure_file(${hname} ${hname} COPYONLY)
endforeach(hname)

configure_file (
  "include/EngineConfig.hpp.in"
  "include/EngineConfig.hpp"
)

include_directories(build/include/)
include_directories(/usr/local/GLFW/include)


add_library(CKEngine STATIC ${SRC} ${TP_SRC})
target_link_libraries(CKEngine glfw3 GL assimp ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} X11 Xrandr Xinerama Xi Xxf86vm Xcursor pthread dl)

foreach(hname ${INSTALL_HEADERS})
    message(${hname})
endforeach(hname)

install(TARGETS CKEngine DESTINATION bin)
install (DIRECTORY "build/include/"        
         DESTINATION "bin/include")