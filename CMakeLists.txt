cmake_minimum_required(VERSION 3.5)

include_directories("${CMAKE_BINARY_DIR}/ThirdParty/g3log/include")
include_directories("${CMAKE_SOURCE_DIR}/ThirdParty/g3log/src")

#add_subdirectory(ThirdParty)

#find_program(iwyu_path NAMES include-what-you-use iwyu)
#if(NOT iwyu_path)
#  message(WARNING "Could not find the program include-what-you-use")
#endif()

add_subdirectory(engine)
#set_property(TARGET CKEngine PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

add_subdirectory(testgame)
#set_property(TARGET TestGame PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

add_custom_target(build DEPENDS CKEngine TestGame)
