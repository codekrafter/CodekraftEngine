cmake_minimum_required(VERSION 3.5)

add_subdirectory(engine)

add_subdirectory(testgame)

add_subdirectory(glfw)
target_link_libraries(CKEngine glfw)
target_link_libraries(TestGame glfw)

add_custom_target(build DEPENDS CKEngine TestGame)
