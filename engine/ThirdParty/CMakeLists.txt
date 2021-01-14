function(message)
    if (NOT MESSAGE_QUIET)
        _message(${ARGN})
    endif()
endfunction()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

if(WIN32)
    set(MSVC ON)
endif()

find_library(LIBRT rt)

set(MESSAGE_QUIET ON)
add_subdirectory(g3log)
unset(MESSAGE_QUIET)