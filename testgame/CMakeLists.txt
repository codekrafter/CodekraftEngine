cmake_minimum_required(VERSION 3.5)
project(CKTestGame)

set(CMAKE_INSTALL_PREFIX ..)
set (CMAKE_CXX_STANDARD 11)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB SRC "src/*.cpp")

include_directories(include/)
include_directories(${CMAKE_BINARY_DIR}/engine/include)
#link_directories()

add_executable(TestGame ${SRC})
target_link_libraries(TestGame ${CMAKE_BINARY_DIR}/engine/libCKEngine.a glfw3 GL ${OPENGL_LIBRARIES} X11 Xrandr Xinerama Xi Xxf86vm Xcursor pthread dl)

#install(TARGETS TestGame DESTINATION bin)

add_custom_target(run ALL DEPENDS TestGame)
add_custom_command(TARGET run
    POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/TestGame
    COMMENT "Run the Test Game in ${CMAKE_CURRENT_BINARY_DIR}"
)