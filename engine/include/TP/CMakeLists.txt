cmake_minimum_required (VERSION 3.1.0)
project (LightingTest)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

ADD_DEFINITIONS(
    -std=c++17 # Or -std=c++0x
    # Other flags
)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(include/)

include_directories(/usr/local/GLFW/include)
include_directories(include/glad)
#include_directories()
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

file(GLOB SRC
    "src/*.cpp"
    "src/*.c"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GLAD include/glad/glad.c)
add_executable(LightingTest ${SRC} ${GLAD})
target_link_libraries(LightingTest glfw3 GL assimp ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} X11 Xrandr Xinerama Xi Xxf86vm Xcursor pthread dl)