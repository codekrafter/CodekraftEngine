cmake_minimum_required(VERSION 3.5)
project(CKEngine)

set(ENGINE_VERSION 0.0.1)
set(CMAKE_INSTALL_PREFIX ..)

file(GLOB SRC "src/*.cpp")
file(GLOB HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "include/*.hpp")
foreach(hname ${HEADERS})
    configure_file(${hname} ${hname} COPYONLY)
endforeach(hname)

configure_file (
  "include/EngineConfig.hpp.in"
  "include/EngineConfig.hpp"
)

include_directories(build/include/)

file(GLOB INSTALL_HEADERS "build/include/*.hpp")
add_library(CKEngine STATIC ${SRC})

install(TARGETS CKEngine DESTINATION bin)
install (FILES ${INSTALL_HEADERS}        
         DESTINATION bin/include)