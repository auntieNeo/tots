cmake_minimum_required(VERSION 2.6)
project (Typing_of_the_Stars)
set (Typing_of_the_Stars_VERSION_MAJOR 0)
set (Typing_of_the_Stars_VERSION_MINOR 2)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set (TOTS_SOURCE_DIR src)

set(CMAKE_CXX_FLAGS "-g -Wall")

if(WIN32)
  if(NOT DEFINED ENV{SDL2DIR})
    message(FATAL_ERROR "Please set the SDL2DIR environment variable to the path of the SDL2 library.\n")
  endif()
endif(WIN32)
find_package(SDL2 REQUIRED)

find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)

include_directories(SYSTEM ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})

add_subdirectory("${TOTS_SOURCE_DIR}")
