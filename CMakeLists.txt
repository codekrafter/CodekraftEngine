cmake_minimum_required(VERSION 3.5)

add_subdirectory(${CMAKE_SOURCE_DIR}/engine ${CMAKE_SOURCE_DIR}build/engine/)
add_subdirectory(${CMAKE_SOURCE_DIR}/editor ${CMAKE_SOURCE_DIR}/build/editor/)
add_subdirectory(${CMAKE_SOURCE_DIR}/testgame ${CMAKE_SOURCE_DIR}/build/testgame/)

add_custom_command(OUTPUT "test1" COMMAND hey 'echo "hey"')
