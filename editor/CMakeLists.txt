cmake_minimum_required(VERSION 3.5)
project(CKEditor)

set(EDITOR_VERSION 0.0.1)
set(CMAKE_INSTALL_PREFIX ..)
set (CMAKE_CXX_STANDARD 11)

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

file(GLOB SRC "src/*.cpp")

configure_file (
  "include/EditorConfig.hpp.in"
  "include/EditorConfig.hpp"
)

include_directories(include/)
include_directories(build/include/)
include_directories(../engine/bin/include)
#include_directories(/usr/include/wx-3.1-unofficial/)
#include_directories(/usr/lib/x86_64-linux-gnu/wx/include/base-unicode-3.1-unofficial3/)
link_directories(../engine/bin/)

add_executable(Editor ${SRC})
target_link_libraries(Editor CKEngine.a glfw3 GL assimp ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} X11 Xrandr Xinerama Xi Xxf86vm Xcursor pthread dl)
target_link_libraries(Editor ${wxWidgets_LIBRARIES})

install(TARGETS Editor DESTINATION bin)