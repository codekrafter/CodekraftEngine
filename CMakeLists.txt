cmake_minimum_required(VERSION 3.5)

add_subdirectory(glfw)

add_subdirectory(engine)

add_subdirectory(testgame)

add_custom_target(build DEPENDS CKEngine TestGame)
