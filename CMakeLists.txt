cmake_minimum_required(VERSION 2.6)
project (Typing_of_the_Stars)
set (Typing_of_the_Stars_VERSION_MAJOR 0)
set (Typing_of_the_Stars_VERSION_MINOR 2)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set (TOTS_SOURCE_DIR src)

if(WIN32)
  set(CMAKE_CXX_FLAGS "/EHsc")
else(WIN32)
  set(CMAKE_CXX_FLAGS "-g -Wall")
endif(WIN32)

find_package(SDL2 REQUIRED)

find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS exception filesystem system)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIRS} ${CMAKE_SOURCE_DIR}/vs_libs/glew-1.10.0/lib/Release/Win32 ${CMAKE_SOURCE_DIR}/vs_libs/SDL2-2.0.3/lib/x86)

add_subdirectory("${TOTS_SOURCE_DIR}")
