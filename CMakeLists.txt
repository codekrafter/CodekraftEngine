cmake_minimum_required(VERSION 3.5)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

find_program(iwyu_path NAMES include-what-you-use iwyu)
if(NOT iwyu_path)
  message(FATAL_ERROR "Could not find the program include-what-you-use")
endif()

add_subdirectory(engine)
#set_property(TARGET CKEngine PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

add_subdirectory(testgame)
#set_property(TARGET TestGame PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

add_custom_target(build DEPENDS CKEngine TestGame)
