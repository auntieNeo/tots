cmake_minimum_required(VERSION 2.6)
project (Typing_of_the_Stars)
set (Typing_of_the_Stars_VERSION_MAJOR 0)
set (Typing_of_the_Stars_VERSION_MINOR 2)

set (TOTS_SOURCE_DIR src)

set(CMAKE_CXX_FLAGS "-g -Wall")

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

include_directories(SYSTEM ${SDL2_INCLUDE_DIRS})

add_subdirectory("${TOTS_SOURCE_DIR}")
